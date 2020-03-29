#pragma once
#include "../../Renderer/WaterRenderer.h"
#include "GLFrameBuffers.h"
#include "GLShaderBank.h"
#include "GLMeshData.h"
#include "GLVAO.h"
namespace Lava
{
	namespace OpenGL
	{
		class GLWaterRenderer : public WaterRenderer
		{
		public:
			GLWaterRenderer(Scene* scene);
			~GLWaterRenderer();
			int BindReflectionFbo();
			int BindRefractionFbo();
			void UnbindAll();
			void SetReflectionDimensions(int width, int height);
			void SetRefractionDimensions(int width, int height);
			void InitReflectionAndRefractionFrameBuffers();
			void Configure(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) override;
			void CompleteRender() override;
			void Update(Scene* scene) override;

		private:
			GLFrameBuffers* fbos;
			
			private:
			float* m_planeVertices;
			unsigned int m_planeVertexCount;
		private:
			int m_refractionFbo;
			int m_refractionColorTexId;
			int m_refractionDepthTexId;
			int m_refractionWidth;
			int m_refractionHeight;

		private:
			int m_reflectionFbo;
			int m_reflectionColorTexId;
			int m_reflectionDepthBuffer;
			int m_reflectionWidth;
			int m_reflectionHeight;

		private:
			GLShaderBank* m_shaderBank;
			GLVAO* m_vao;
			std::shared_ptr<GLVertexBuffer> m_vbo;
			VertexBufferElement* m_bufferLayout;
		};
	}
}