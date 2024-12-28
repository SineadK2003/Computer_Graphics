#include <render/headers.h>
#include <render/shader.h>

#include <fp_skybox.cpp>
static float FoV = 60.0f;
static float zNear = 100.0f;
static float zFar = 1500.0f; 

// Lighting  
static glm::vec3 lightIntensity(5e6f, 5e6f, 5e6f);
static glm::vec3 lightPosition(-275.0f, 500.0f, 800.0f);

// Animation 
static bool playAnimation = true;
static float playbackSpeed = 2.0f;

struct MyBot {
	// Shader variable IDs
	GLuint mvpMatrixID;
	GLuint jointMatricesID;
	GLuint lightPositionID;
	GLuint lightIntensityID;
	GLuint programID;

	tinygltf::Model model;

	// Each VAO corresponds to each mesh primitive in the GLTF model
	struct PrimitiveObject {
		GLuint vao;
		std::map<int, GLuint> vbos;
	};
	std::vector<PrimitiveObject> primitiveObjects;

	// Skinning 
	struct SkinObject {
		// Transforms the geometry into the space of the respective joint
		std::vector<glm::mat4> inverseBindMatrices;  

		// Transforms the geometry following the movement of the joints
		std::vector<glm::mat4> globalJointTransforms;

		// Combined transforms
		std::vector<glm::mat4> jointMatrices;
	};
	std::vector<SkinObject> skinObjects;

	// Animation 
	struct SamplerObject {
		std::vector<float> input;
		std::vector<glm::vec3> outputVec3;
		std::vector<glm::vec4> outputVec4;
		int interpolation;
	};

	struct NodeState {
    glm::vec3 translation;
    glm::quat rotation;
    glm::vec3 scale;
	};

	std::vector<NodeState> nodeStates;

	struct ChannelObject {
		int sampler;
		std::string targetPath;
		int targetNode;
	}; 
	struct AnimationObject {
		std::vector<SamplerObject> samplers;	// Animation data
	};

	std::vector<AnimationObject> animationObjects;

	glm::mat4 getNodeTransform(const tinygltf::Node& node) {
		glm::mat4 transform(1.0f); 

		if (node.matrix.size() == 16) {
			transform = glm::make_mat4(node.matrix.data());
		} else {
			if (node.translation.size() == 3) {
				transform = glm::translate(transform, glm::vec3(node.translation[0], node.translation[1], node.translation[2]));
			}
			if (node.rotation.size() == 4) {
				glm::quat q(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
				transform *= glm::mat4_cast(q);
			}
			if (node.scale.size() == 3) {
				transform = glm::scale(transform, glm::vec3(node.scale[0], node.scale[1], node.scale[2]));
			}
		}
		return transform;
	}

	void computeLocalNodeTransform(const tinygltf::Model& model, 
		int nodeIndex, 
		std::vector<glm::mat4> &localTransforms)
	{
		const tinygltf::Node &node = model.nodes[nodeIndex];

		// get local transform for the current node
		localTransforms[nodeIndex] = getNodeTransform(node);

		// recursively compute transforms for all child nodes
		for (int childIndex : node.children) {
			computeLocalNodeTransform(model, childIndex, localTransforms);
		}
	}

	void computeGlobalNodeTransform(const tinygltf::Model& model, 
		const std::vector<glm::mat4> &localTransforms,
		int nodeIndex, const glm::mat4& parentTransform, 
		std::vector<glm::mat4> &globalTransforms)
	{
		// compute global transform for the current node
		globalTransforms[nodeIndex] = parentTransform * localTransforms[nodeIndex];

		// recursively compute global transforms for child nodes
		const tinygltf::Node &node = model.nodes[nodeIndex];
		for (int childIndex : node.children) {
			computeGlobalNodeTransform(model, localTransforms, childIndex, globalTransforms[nodeIndex], globalTransforms);
		}
	}

	std::vector<SkinObject> prepareSkinning(const tinygltf::Model &model) {
		std::vector<SkinObject> skinObjects;

		// In our Blender exporter, the default number of joints that may influence a vertex is set to 4, just for convenient implementation in shaders.

		for (size_t i = 0; i < model.skins.size(); i++) {
			SkinObject skinObject;

			const tinygltf::Skin &skin = model.skins[i];

			// Read inverseBindMatrices
			const tinygltf::Accessor &accessor = model.accessors[skin.inverseBindMatrices];
			assert(accessor.type == TINYGLTF_TYPE_MAT4);
			const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
			const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
			const float *ptr = reinterpret_cast<const float *>(
            	buffer.data.data() + accessor.byteOffset + bufferView.byteOffset);
			
			skinObject.inverseBindMatrices.resize(accessor.count);
			for (size_t j = 0; j < accessor.count; j++) {
				float m[16];
				memcpy(m, ptr + j * 16, 16 * sizeof(float));
				skinObject.inverseBindMatrices[j] = glm::make_mat4(m);
			}

			assert(skin.joints.size() == accessor.count);

			skinObject.globalJointTransforms.resize(skin.joints.size());
			skinObject.jointMatrices.resize(skin.joints.size());

			// Compute local transforms for all nodes
			std::vector<glm::mat4> localTransforms(model.nodes.size(), glm::mat4(1.0f));
			for (size_t i = 0; i < model.nodes.size(); ++i) {
				localTransforms[i] = getNodeTransform(model.nodes[i]);
			}

			// Compute global transforms starting from the scene's root nodes
			std::vector<glm::mat4> globalTransforms(model.nodes.size(), glm::mat4(1.0f));
			for (size_t i = 0; i < model.scenes[model.defaultScene].nodes.size(); ++i) {
				computeGlobalNodeTransform(model, localTransforms, model.scenes[model.defaultScene].nodes[i], glm::mat4(1.0f), globalTransforms);
			}

			// Store global joint transforms
			skinObject.globalJointTransforms.resize(model.nodes.size(), glm::mat4(1.0f));
			for (size_t j = 0; j < skin.joints.size(); ++j) {
				int nodeIndex = skin.joints[j];
				skinObject.globalJointTransforms[nodeIndex] = globalTransforms[nodeIndex];
			}
			skinObjects.push_back(skinObject);
		}

		return skinObjects;
	}

	int findKeyframeIndex(const std::vector<float>& times, float animationTime) 
	{
		int left = 0;
		int right = times.size() - 1;

		while (left <= right) {
			int mid = (left + right) / 2;

			if (mid + 1 < times.size() && times[mid] <= animationTime && animationTime < times[mid + 1]) {
				return mid;
			}
			else if (times[mid] > animationTime) {
				right = mid - 1;
			}
			else { // animationTime >= times[mid + 1]
				left = mid + 1;
			}
		}

		// Target not found
		return times.size() - 2;
	}

	std::vector<AnimationObject> prepareAnimation(const tinygltf::Model &model) 
{
    std::vector<AnimationObject> animationObjects;
    for (const auto &anim : model.animations) {
        AnimationObject animationObject;
        
        for (const auto &sampler : anim.samplers) {
            SamplerObject samplerObject;

            // Read input times
            const tinygltf::Accessor &inputAccessor = model.accessors[sampler.input];
            const tinygltf::BufferView &inputBufferView = model.bufferViews[inputAccessor.bufferView];
            const tinygltf::Buffer &inputBuffer = model.buffers[inputBufferView.buffer];

            const unsigned char *inputPtr = &inputBuffer.data[inputBufferView.byteOffset + inputAccessor.byteOffset];
            int inputStride = inputAccessor.ByteStride(inputBufferView);

            samplerObject.input.resize(inputAccessor.count);
            for (size_t i = 0; i < inputAccessor.count; ++i) {
                const float *p = reinterpret_cast<const float*>(inputPtr + i * inputStride);
                samplerObject.input[i] = *p;
            }

            // Read output values
            const tinygltf::Accessor &outputAccessor = model.accessors[sampler.output];
            const tinygltf::BufferView &outputBufferView = model.bufferViews[outputAccessor.bufferView];
            const tinygltf::Buffer &outputBuffer = model.buffers[outputBufferView.buffer];

            const unsigned char *outputPtr = &outputBuffer.data[outputBufferView.byteOffset + outputAccessor.byteOffset];
            int outputStride = outputAccessor.ByteStride(outputBufferView);

            if (outputAccessor.type == TINYGLTF_TYPE_VEC3) {
                // VEC3 data (translation or scale)
                samplerObject.outputVec3.resize(outputAccessor.count);
                for (size_t i = 0; i < outputAccessor.count; ++i) {
                    const float *p = reinterpret_cast<const float*>(outputPtr + i * outputStride);
                    samplerObject.outputVec3[i] = glm::vec3(p[0], p[1], p[2]);
                }
            } else if (outputAccessor.type == TINYGLTF_TYPE_VEC4) {
                // VEC4 data (rotation)
                samplerObject.outputVec4.resize(outputAccessor.count);
                for (size_t i = 0; i < outputAccessor.count; ++i) {
                    const float *p = reinterpret_cast<const float*>(outputPtr + i * outputStride);
                    samplerObject.outputVec4[i] = glm::vec4(p[0], p[1], p[2], p[3]);
                }
            } else {
                std::cout << "Unsupported accessor type in animation output" << std::endl;
            }

            animationObject.samplers.push_back(samplerObject);           
        }

        animationObjects.push_back(animationObject);
    }
    return animationObjects;
}

	

	void updateAnimation(
    const tinygltf::Model &model, 
    const tinygltf::Animation &anim, 
    const AnimationObject &animationObject, 
    float time)
{
    for (size_t i = 0; i < anim.channels.size(); ++i) {
        const auto &channel = anim.channels[i];
        int targetNodeIndex = channel.target_node;
        const auto &sampler = anim.samplers[channel.sampler];

        const SamplerObject &samplerObject = animationObject.samplers[channel.sampler];

        const std::vector<float> &times = samplerObject.input;
        float animationTime = fmod(time, times.back());

        int keyframeIndex = findKeyframeIndex(times, animationTime);
        float t = (animationTime - times[keyframeIndex]) / 
                (times[keyframeIndex + 1] - times[keyframeIndex]);

        NodeState &state = nodeStates[targetNodeIndex];

        if (channel.target_path == "translation") {
            glm::vec3 translation0 = samplerObject.outputVec3[keyframeIndex];
            glm::vec3 translation1 = samplerObject.outputVec3[keyframeIndex + 1];

            glm::vec3 translation = glm::mix(translation0, translation1, t);
            state.translation = translation;
        } else if (channel.target_path == "rotation") {
            glm::quat rotation0 = glm::quat(samplerObject.outputVec4[keyframeIndex].w, samplerObject.outputVec4[keyframeIndex].x, samplerObject.outputVec4[keyframeIndex].y, samplerObject.outputVec4[keyframeIndex].z);
            glm::quat rotation1 = glm::quat(samplerObject.outputVec4[keyframeIndex + 1].w, samplerObject.outputVec4[keyframeIndex + 1].x, samplerObject.outputVec4[keyframeIndex + 1].y, samplerObject.outputVec4[keyframeIndex + 1].z);

            glm::quat rotation = glm::slerp(rotation0, rotation1, t);
            state.rotation = rotation;
        } else if (channel.target_path == "scale") {
            glm::vec3 scale0 = samplerObject.outputVec3[keyframeIndex];
            glm::vec3 scale1 = samplerObject.outputVec3[keyframeIndex + 1];

            glm::vec3 scale = glm::mix(scale0, scale1, t);
            state.scale = scale;
        }
    }
}


	void updateSkinning(const std::vector<glm::mat4> &globalTransforms) {
    for (auto &skinObject : skinObjects) {
        for (size_t j = 0; j < skinObject.jointMatrices.size(); ++j) {
            int nodeIndex = model.skins[0].joints[j];  // Assuming single skin
            skinObject.jointMatrices[j] = globalTransforms[nodeIndex] * skinObject.inverseBindMatrices[j];
        }
    }
}


	void update(float time) {
    if (model.animations.size() > 0) {
        const tinygltf::Animation &animation = model.animations[0];
        const AnimationObject &animationObject = animationObjects[0];

        // Apply animations to nodeStates
        updateAnimation(model, animation, animationObject, time);

        // Reconstruct nodeTransforms from nodeStates
        std::vector<glm::mat4> nodeTransforms(model.nodes.size(), glm::mat4(1.0f));
        for (size_t i = 0; i < model.nodes.size(); ++i) {
            NodeState &state = nodeStates[i];
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), state.translation);
            glm::mat4 rotationMatrix = glm::mat4_cast(state.rotation);
            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), state.scale);

            nodeTransforms[i] = translationMatrix * rotationMatrix * scaleMatrix;
        }

        // Compute global transforms
        std::vector<glm::mat4> globalTransforms(model.nodes.size(), glm::mat4(1.0f));
        for (size_t i = 0; i < model.scenes[model.defaultScene].nodes.size(); ++i) {
            computeGlobalNodeTransform(model, nodeTransforms,
                model.scenes[model.defaultScene].nodes[i], glm::mat4(1.0f), globalTransforms);
        }

        // Recompute joint matrices for skinning
        updateSkinning(globalTransforms);
    }
}


	bool loadModel(tinygltf::Model &model, const char *filename) {
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		bool res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
		if (!warn.empty()) {
			std::cout << "WARN: " << warn << std::endl;
		}

		if (!err.empty()) {
			std::cout << "ERR: " << err << std::endl;
		}

		if (!res)
			std::cout << "Failed to load glTF: " << filename << std::endl;
		else
			std::cout << "Loaded glTF: " << filename << std::endl;

		return res;
	}

	void initialize() {
		// Modify your path if needed
		if (!loadModel(model, "../lab4/model/bot/bot.gltf")) {
			return;
		}

		// Prepare buffers for rendering 
		primitiveObjects = bindModel(model);

		// Prepare joint matrices
		skinObjects = prepareSkinning(model);

		// Prepare animation data 
		animationObjects = prepareAnimation(model);

		// Create and compile our GLSL program from the shaders
		programID = LoadShadersFromFile("../lab4/shader/bot.vert", "../lab4/shader/bot.frag");
		if (programID == 0)
		{
			std::cerr << "Failed to load shaders." << std::endl;
		}

		nodeStates.resize(model.nodes.size());
    for (size_t i = 0; i < model.nodes.size(); ++i) {
        const tinygltf::Node &node = model.nodes[i];

        NodeState &state = nodeStates[i];
        // Translation
        if (node.translation.size() == 3) {
            state.translation = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
        } else {
            state.translation = glm::vec3(0.0f);
        }
        // Rotation
        if (node.rotation.size() == 4) {
            state.rotation = glm::quat(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
        } else {
            state.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        }
        // Scale
        if (node.scale.size() == 3) {
            state.scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
        } else {
            state.scale = glm::vec3(1.0f);
        }
    }

		// Get a handle for GLSL variables
		mvpMatrixID = glGetUniformLocation(programID, "MVP");
		lightPositionID = glGetUniformLocation(programID, "lightPosition");
		lightIntensityID = glGetUniformLocation(programID, "lightIntensity");
		jointMatricesID = glGetUniformLocation(programID, "jointMatrices");
	}

	void bindMesh(std::vector<PrimitiveObject> &primitiveObjects,
				tinygltf::Model &model, tinygltf::Mesh &mesh) {

		std::map<int, GLuint> vbos;
		for (size_t i = 0; i < model.bufferViews.size(); ++i) {
			const tinygltf::BufferView &bufferView = model.bufferViews[i];

			int target = bufferView.target;
			
			if (bufferView.target == 0) { 
				// The bufferView with target == 0 in our model refers to 
				// the skinning weights, for 25 joints, each 4x4 matrix (16 floats), totaling to 400 floats or 1600 bytes. 
				// So it is considered safe to skip the warning.
				//std::cout << "WARN: bufferView.target is zero" << std::endl;
				continue;
			}

			const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
			GLuint vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(target, vbo);
			glBufferData(target, bufferView.byteLength,
						&buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
			
			vbos[i] = vbo;
		}

		// Each mesh can contain several primitives (or parts), each we need to 
		// bind to an OpenGL vertex array object
		for (size_t i = 0; i < mesh.primitives.size(); ++i) {

			tinygltf::Primitive primitive = mesh.primitives[i];
			tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

			GLuint vao;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			for (auto &attrib : primitive.attributes) {
				tinygltf::Accessor accessor = model.accessors[attrib.second];
				int byteStride =
					accessor.ByteStride(model.bufferViews[accessor.bufferView]);
				glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

				int size = 1;
				if (accessor.type != TINYGLTF_TYPE_SCALAR) {
					size = accessor.type;
				}

			int vaa = -1;
            if (attrib.first == "POSITION") vaa = 0;
            else if (attrib.first == "NORMAL") vaa = 1;
            else if (attrib.first == "TEXCOORD_0") vaa = 2;
            else if (attrib.first == "JOINTS_0") vaa = 3;
            else if (attrib.first == "WEIGHTS_0") vaa = 4;

            if (vaa > -1) {
                glEnableVertexAttribArray(vaa);

                if (attrib.first == "JOINTS_0") {
                    // integer attribute pointer for joint indices
                    glVertexAttribIPointer(vaa, size, accessor.componentType,
                                           byteStride, BUFFER_OFFSET(accessor.byteOffset));
                } else {
                    // regular attribute pointer for other attributes
                    glVertexAttribPointer(vaa, size, accessor.componentType,
                                          accessor.normalized ? GL_TRUE : GL_FALSE,
                                          byteStride, BUFFER_OFFSET(accessor.byteOffset));
                }
            } else {
                std::cout << "Unknown attribute: " << attrib.first << std::endl;
            }
        }

        PrimitiveObject primitiveObject;
        primitiveObject.vao = vao;
        primitiveObject.vbos = vbos;
        primitiveObjects.push_back(primitiveObject);

        glBindVertexArray(0);
    }
}

	void bindModelNodes(std::vector<PrimitiveObject> &primitiveObjects, 
						tinygltf::Model &model,
						tinygltf::Node &node) {
		// Bind buffers for the current mesh at the node
		if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
			bindMesh(primitiveObjects, model, model.meshes[node.mesh]);
		}

		// Recursive into children nodes
		for (size_t i = 0; i < node.children.size(); i++) {
			assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
			bindModelNodes(primitiveObjects, model, model.nodes[node.children[i]]);
		}
	}

	std::vector<PrimitiveObject> bindModel(tinygltf::Model &model) {
		std::vector<PrimitiveObject> primitiveObjects;

		const tinygltf::Scene &scene = model.scenes[model.defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); ++i) {
			assert((scene.nodes[i] >= 0) && (scene.nodes[i] < model.nodes.size()));
			bindModelNodes(primitiveObjects, model, model.nodes[scene.nodes[i]]);
		}

		return primitiveObjects;
	}

	void drawMesh(const std::vector<PrimitiveObject> &primitiveObjects,
				tinygltf::Model &model, tinygltf::Mesh &mesh) {
		
		for (size_t i = 0; i < mesh.primitives.size(); ++i) 
		{
			GLuint vao = primitiveObjects[i].vao;
			std::map<int, GLuint> vbos = primitiveObjects[i].vbos;

			glBindVertexArray(vao);

			tinygltf::Primitive primitive = mesh.primitives[i];
			tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.at(indexAccessor.bufferView));

			glDrawElements(primitive.mode, indexAccessor.count,
						indexAccessor.componentType,
						BUFFER_OFFSET(indexAccessor.byteOffset));

			glBindVertexArray(0);
		}
	}

	void drawModelNodes(const std::vector<PrimitiveObject>& primitiveObjects,
						tinygltf::Model &model, tinygltf::Node &node) {
		// Draw the mesh at the node, and recursively do so for children nodes
		if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
			drawMesh(primitiveObjects, model, model.meshes[node.mesh]);
		}
		for (size_t i = 0; i < node.children.size(); i++) {
			drawModelNodes(primitiveObjects, model, model.nodes[node.children[i]]);
		}
	}
	void drawModel(const std::vector<PrimitiveObject>& primitiveObjects,
				tinygltf::Model &model) {
		// Draw all nodes
		const tinygltf::Scene &scene = model.scenes[model.defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); ++i) {
			drawModelNodes(primitiveObjects, model, model.nodes[scene.nodes[i]]);
		}
	}

	void render(glm::mat4 cameraMatrix) {
		glUseProgram(programID);
		
		// Set camera
		glm::mat4 mvp = cameraMatrix;
		glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);

		// Set light data 
		glUniform3fv(lightPositionID, 1, &lightPosition[0]);
		glUniform3fv(lightIntensityID, 1, &lightIntensity[0]);

		for (const auto &skinObject : skinObjects) {
        glUniformMatrix4fv(jointMatricesID, skinObject.jointMatrices.size(), GL_FALSE, 
                           glm::value_ptr(skinObject.jointMatrices[0]));
    }


		// Draw the GLTF model
		drawModel(primitiveObjects, model);
	}
	
	void cleanup() {
		glDeleteProgram(programID);
	}


}; 


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW." << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MacOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(windowWidth, windowHeight, "Lab 4", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to open a GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(window, key_callback);

	// Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0)
	{
		std::cerr << "Failed to initialize OpenGL context." << std::endl;
		return -1;
	}

	// Background
	glClearColor(0.2f, 0.2f, 0.25f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Our 3D character
	MyBot bot;
	bot.initialize();

	// Camera setup
    glm::mat4 viewMatrix, projectionMatrix;
	projectionMatrix = glm::perspective(glm::radians(FoV), (float)windowWidth / windowHeight, zNear, zFar);

	// Time and frame rate tracking
	static double lastTime = glfwGetTime();
	float time = 0.0f;			// Animation time 
	float fTime = 0.0f;			// Time for measuring fps
	unsigned long frames = 0;

	// Main loop
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update states for animation
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
		lastTime = currentTime;

		if (playAnimation) {
			time += deltaTime * playbackSpeed;
			bot.update(time);
		}

		// Rendering
		viewMatrix = glm::lookAt(eye_center, lookat, up);
		glm::mat4 vp = projectionMatrix * viewMatrix;
		bot.render(vp);

		// FPS tracking 
		// Count number of frames over a few seconds and take average
		frames++;
		fTime += deltaTime;
		if (fTime > 2.0f) {		
			float fps = frames / fTime;
			frames = 0;
			fTime = 0;
			
			std::stringstream stream;
			stream << std::fixed << std::setprecision(2) << "Lab 4 | Frames per second (FPS): " << fps;
			glfwSetWindowTitle(window, stream.str().c_str());
		}

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (!glfwWindowShouldClose(window));

	// Clean up
	bot.cleanup();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		playbackSpeed += 1.0f;
		if (playbackSpeed > 10.0f) 
			playbackSpeed = 10.0f;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		playbackSpeed -= 1.0f;
		if (playbackSpeed < 1.0f) {
			playbackSpeed = 1.0f;
		}
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		playAnimation = !playAnimation;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}