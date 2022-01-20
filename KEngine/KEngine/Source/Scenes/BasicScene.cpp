#include "stdafx.h"

#include "BasicScene.h"

#include "Game.h"
#include "RenderSystem.h"
#include "KWindow.h"
#include "Objects/BasicObject.h"
#include "Objects/Lights.h"
#include "Objects/TexturedCube.h"
#include "Objects/ModeledObject.h"

#include "GLShader.h"
#include "MeshData.h"

#include "stb_image.h"

#include "Model.h"

Model* model = nullptr;

//----------------------------------------------------------
BasicScene::BasicScene(Game* game)
:	IScene(game)
,	m_sceneMode(ESceneMode::Normal)
{
}

//----------------------------------------------------------
BasicScene::~BasicScene()
{
	for (int i = 0; i < m_lightCastedObjects.size(); i++)
		delete m_lightCastedObjects[i];

	for (int i = 0; i < m_Objects.size(); i++)
		delete m_Objects[i];

	for (int i = 0; i < m_pointLights.size(); i++)
		delete m_pointLights[i];

	for (int i = 0; i < m_spotLights.size(); i++)
		delete m_spotLights[i];
	
	delete m_dirLight;
}

//----------------------------------------------------------
BasicScene::BasicScene(BasicScene const& other)
:	super(other)
{}

//----------------------------------------------------------
IScene* BasicScene::Clone()
{
	BasicScene* clone = new BasicScene(*this);
	return clone;
}

//----------------------------------------------------------
void	BasicScene::BuildScene()
{
	{
		IObject* obj = new TexturedCube();
		obj->SetMeshName("TexturedCube");
		obj->SetShaderName("TexturedMeshShader");
		obj->Position() = glm::vec3(0.f, 0.f, 0.f);
		m_Objects.push_back(obj);
	}

	glm::vec3 cubePositions[] =
	{
		glm::vec3(0.0f,  -3.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-3.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -3.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  3.2f, -1.5f),
		glm::vec3(-3.3f,  1.0f, -2.5f)
	};

	for (unsigned int i = 0; i < 10; i++)
	{
		float angle = 20.0f * i;
		ModeledObject* lightCastedObject = new ModeledObject();

		//lightCastedObject->SetMeshName("SimpleCube_Normal");
		lightCastedObject->SetShaderName("SimpleLightCastedObject");

		lightCastedObject->SetModelName("ContainerCube");

		//ContainerCube

		lightCastedObject->Position() = cubePositions[i];
		float rad = glm::radians(angle);
		lightCastedObject->Rotation() = glm::vec3(1.0f * rad, 0.3f * rad, 0.5f * rad);

		m_lightCastedObjects.push_back(lightCastedObject);
	}

	ModeledObject* lightCastedObject = new ModeledObject();
	//lightCastedObject->SetMeshName("SimpleCube_Normal");
	lightCastedObject->SetModelName("ContainerCube");
	lightCastedObject->SetShaderName("SimpleLightCastedObject");
	lightCastedObject->Position() = glm::vec3(0.f, -30.f, 0.f);
	lightCastedObject->Scale() = glm::vec3(50.f);

	m_lightCastedObjects.push_back(lightCastedObject);

	CreateLights();

	for (int i = 0; i < m_Objects.size(); i++)
		m_Objects[i]->OnSceneBuild();

	// to remove
	for (int i = 0; i < m_pointLights.size(); i++)
		m_pointLights[i]->OnSceneStart();
	for (int i = 0; i < m_spotLights.size(); i++)
		m_spotLights[i]->OnSceneStart();

}

//----------------------------------------------------------
void	BasicScene::OnSceneStart()
{
	// not implemented

	for (int i = 0; i < m_Objects.size(); i++)
		m_Objects[i]->OnSceneStart();

	for (int i = 0; i < m_pointLights.size(); i++)
		m_pointLights[i]->OnSceneStart();
}

//----------------------------------------------------------
void	BasicScene::Update(float dt)
{
	super::Update(dt);

	for (int i = 0; i < m_Objects.size(); i++)
		m_Objects[i]->Update(dt);

	for (int i = 0; i < m_pointLights.size(); i++)
		m_pointLights[i]->Update(dt);

	for (int i = 0; i < m_spotLights.size(); i++)
		m_spotLights[i]->Update(dt);
}

//----------------------------------------------------------
void	BasicScene::Render()
{
	RenderSystem* rS = m_game->GetRenderSystem();

	glm::vec3	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	GLShader*	lightShader = rS->GetShader(m_pointLights[0]->ShaderName());
	GLShader*	overrideShader = m_sceneMode == ESceneMode::Depth ? rS->GetShader("Depth") : nullptr;


	model = rS->GetModel("backpack");// new Model(ModelPath("backpack/backpack.obj").c_str());

	{
		m_camera.SetOrthoMat(glm::ortho(-400.f, 400.0f, -300.f, 300.0f, 0.1f, 200.0f));
		m_camera.SetProjMat(glm::perspective(glm::radians(m_camera.Fov()), (float)m_game->GetRenderWindow()->Width() / (float)m_game->GetRenderWindow()->Height(), 0.1f, 200.0f));
	}
	//for (int i = 0; i < m_Objects.size(); i++)
	//	m_Objects[i]->Render(rS, &m_camera);


	for (int i = 0; i < m_pointLights.size(); i++)
		m_pointLights[i]->Render(rS, &m_camera, overrideShader ? overrideShader : lightShader);

	for (int i = 0; i < m_spotLights.size(); i++)
		m_spotLights[i]->Render(rS, &m_camera, overrideShader ? overrideShader : lightShader);

	// render lightCastedObject
	{
		TextureResource*	container2tex = rS->GetTexture("container2"); // diffuseMap
		TextureResource*	container2tex_specular = rS->GetTexture("container2_specularMap"); // specularMap

		for (int i = 0; i < m_lightCastedObjects.size(); i++)
		{
			glm::mat4			modelW = m_lightCastedObjects[i]->ModelWorldMatrix();
			//MeshData*			mesh = rS->GetMesh(m_lightCastedObjects[i]->MeshName());
			Model*				model = rS->GetModel(m_lightCastedObjects[i]->ModelName());

			if (overrideShader)
			{
				overrideShader->Bind();

				glUniformMatrix4fv(overrideShader->Uniform("model"), 1, GL_FALSE, glm::value_ptr(modelW));
				glUniformMatrix4fv(overrideShader->Uniform("view"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewM()));
				glUniformMatrix4fv(overrideShader->Uniform("proj"), 1, GL_FALSE, glm::value_ptr(m_camera.ProjMat()));

				model->Draw(overrideShader);

				//glBindVertexArray(model ->VAO());
				//glDrawArrays(mesh->Mode(), 0, mesh->VerticesNbr());
				//glBindVertexArray(0);
				//glEnableVertexAttribArray(0);
				overrideShader->Unbind();
			}
			else
			{
				GLShader*			shader = rS->GetShader(m_lightCastedObjects[i]->ShaderName());
				shader->Bind();

				glUniformMatrix4fv(shader->Uniform("model"), 1, GL_FALSE, glm::value_ptr(modelW));
				glUniformMatrix4fv(shader->Uniform("view"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewM()));
				glUniformMatrix4fv(shader->Uniform("proj"), 1, GL_FALSE, glm::value_ptr(m_camera.ProjMat()));

				glUniform3f(shader->Uniform("viewPos"), m_camera.Position().x, m_camera.Position().y, m_camera.Position().z);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, container2tex->TextureRenderId());
				glUniform1i(glGetUniformLocation(shader->ProgramID(), "material.diffuse"), 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, container2tex_specular->TextureRenderId());
				glUniform1i(glGetUniformLocation(shader->ProgramID(), "material.specular"), 1);

				glUniform1f(shader->Uniform("material.shininess"), 32.f);

				glUniform3f(shader->Uniform("dirLight.direction"), m_dirLight->m_direction.x, m_dirLight->m_direction.y, m_dirLight->m_direction.z);
				glUniform3f(shader->Uniform("dirLight.ambient"), m_dirLight->m_ambient.x, m_dirLight->m_ambient.y, m_dirLight->m_ambient.z);
				glUniform3f(shader->Uniform("dirLight.diffuse"), m_dirLight->m_diffuse.x, m_dirLight->m_diffuse.y, m_dirLight->m_diffuse.z);
				glUniform3f(shader->Uniform("dirLight.specular"), m_dirLight->m_specular.x, m_dirLight->m_specular.y, m_dirLight->m_specular.z);

				for (int i = 0; i < m_pointLights.size(); i++)
				{
					std::string number = std::to_string(i);
					glUniform3f(shader->Uniform(("pointLights[" + number + "].position").c_str()), m_pointLights[i]->Position().x, m_pointLights[i]->Position().y, m_pointLights[i]->Position().z);
					glUniform3f(shader->Uniform(("pointLights[" + number + "].ambient").c_str()), m_pointLights[i]->m_ambient.x, m_pointLights[i]->m_ambient.y, m_pointLights[i]->m_ambient.z);
					glUniform3f(shader->Uniform(("pointLights[" + number + "].diffuse").c_str()), m_pointLights[i]->m_diffuse.x, m_pointLights[i]->m_diffuse.y, m_pointLights[i]->m_diffuse.z);
					glUniform3f(shader->Uniform(("pointLights[" + number + "].specular").c_str()), m_pointLights[i]->m_specular.x, m_pointLights[i]->m_specular.y, m_pointLights[i]->m_specular.z);
					glUniform1f(shader->Uniform(("pointLights[" + number + "].constant").c_str()), m_pointLights[i]->m_constant);
					glUniform1f(shader->Uniform(("pointLights[" + number + "].linear").c_str()), m_pointLights[i]->m_linear);
					glUniform1f(shader->Uniform(("pointLights[" + number + "].quadratic").c_str()), m_pointLights[i]->m_quadratic);
				}

				for (int i = 0; i < m_spotLights.size(); i++)
				{
					std::string number = std::to_string(i);
					glUniform3f(shader->Uniform(("spotLights[" + number + "].position").c_str()), m_spotLights[i]->Position().x, m_spotLights[i]->Position().y, m_spotLights[i]->Position().z);
					glUniform3f(shader->Uniform(("spotLights[" + number + "].direction").c_str()), m_spotLights[i]->m_direction.x, m_spotLights[i]->m_direction.y, m_spotLights[i]->m_direction.z);
					glUniform3f(shader->Uniform(("spotLights[" + number + "].ambient").c_str()), m_spotLights[i]->m_ambient.x, m_spotLights[i]->m_ambient.y, m_spotLights[i]->m_ambient.z);
					glUniform3f(shader->Uniform(("spotLights[" + number + "].diffuse").c_str()), m_spotLights[i]->m_diffuse.x, m_spotLights[i]->m_diffuse.y, m_spotLights[i]->m_diffuse.z);
					glUniform3f(shader->Uniform(("spotLights[" + number + "].specular").c_str()), m_spotLights[i]->m_specular.x, m_spotLights[i]->m_specular.y, m_spotLights[i]->m_specular.z);
					glUniform1f(shader->Uniform(("spotLights[" + number + "].constant").c_str()), m_spotLights[i]->m_constant);
					glUniform1f(shader->Uniform(("spotLights[" + number + "].linear").c_str()), m_spotLights[i]->m_linear);
					glUniform1f(shader->Uniform(("spotLights[" + number + "].quadratic").c_str()), m_spotLights[i]->m_quadratic);
					glUniform1f(shader->Uniform(("spotLights[" + number + "].cutoff").c_str()), m_spotLights[i]->m_cutoff);
					glUniform1f(shader->Uniform(("spotLights[" + number + "].outerCutoff").c_str()), m_spotLights[i]->m_outerCutoff);
				}

				model->Draw(shader);

				//glBindVertexArray(mesh->VAO());
				//
				////glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO()); // is it necessary to couple that with glBindVertexArray ?
				//glDrawArrays(mesh->Mode(), 0, mesh->VerticesNbr());
				//glBindVertexArray(0);
				//glEnableVertexAttribArray(0);
				shader->Unbind();
			}
		}
	}

	{
		GLShader* shader = overrideShader ? overrideShader : rS->GetShader("SimpleModel");
		glm::mat4			modelW = m_Objects[0]->ModelWorldMatrix();

		shader->Bind();			
		glUniformMatrix4fv(shader->Uniform("model"), 1, GL_FALSE, glm::value_ptr(modelW));
		glUniformMatrix4fv(shader->Uniform("view"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewM()));
		glUniformMatrix4fv(shader->Uniform("proj"), 1, GL_FALSE, glm::value_ptr(m_camera.ProjMat()));

		glUniform3f(shader->Uniform("viewPos"), m_camera.Position().x, m_camera.Position().y, m_camera.Position().z);

		glUniform1f(shader->Uniform("material.shininess"), 32.f);

		glUniform3f(shader->Uniform("dirLight.direction"), m_dirLight->m_direction.x, m_dirLight->m_direction.y, m_dirLight->m_direction.z);
		glUniform3f(shader->Uniform("dirLight.ambient"), m_dirLight->m_ambient.x, m_dirLight->m_ambient.y, m_dirLight->m_ambient.z);
		glUniform3f(shader->Uniform("dirLight.diffuse"), m_dirLight->m_diffuse.x, m_dirLight->m_diffuse.y, m_dirLight->m_diffuse.z);
		glUniform3f(shader->Uniform("dirLight.specular"), m_dirLight->m_specular.x, m_dirLight->m_specular.y, m_dirLight->m_specular.z);

		for (int i = 0; i < m_pointLights.size(); i++)
		{
			std::string number = std::to_string(i);
			glUniform3f(shader->Uniform(("pointLights[" + number + "].position").c_str()), m_pointLights[i]->Position().x, m_pointLights[i]->Position().y, m_pointLights[i]->Position().z);
			glUniform3f(shader->Uniform(("pointLights[" + number + "].ambient").c_str()), m_pointLights[i]->m_ambient.x, m_pointLights[i]->m_ambient.y, m_pointLights[i]->m_ambient.z);
			glUniform3f(shader->Uniform(("pointLights[" + number + "].diffuse").c_str()), m_pointLights[i]->m_diffuse.x, m_pointLights[i]->m_diffuse.y, m_pointLights[i]->m_diffuse.z);
			glUniform3f(shader->Uniform(("pointLights[" + number + "].specular").c_str()), m_pointLights[i]->m_specular.x, m_pointLights[i]->m_specular.y, m_pointLights[i]->m_specular.z);
			glUniform1f(shader->Uniform(("pointLights[" + number + "].constant").c_str()), m_pointLights[i]->m_constant);
			glUniform1f(shader->Uniform(("pointLights[" + number + "].linear").c_str()), m_pointLights[i]->m_linear);
			glUniform1f(shader->Uniform(("pointLights[" + number + "].quadratic").c_str()), m_pointLights[i]->m_quadratic);
		}

		for (int i = 0; i < m_spotLights.size(); i++)
		{
			std::string number = std::to_string(i);
			glUniform3f(shader->Uniform(("spotLights[" + number + "].position").c_str()), m_spotLights[i]->Position().x, m_spotLights[i]->Position().y, m_spotLights[i]->Position().z);
			glUniform3f(shader->Uniform(("spotLights[" + number + "].direction").c_str()), m_spotLights[i]->m_direction.x, m_spotLights[i]->m_direction.y, m_spotLights[i]->m_direction.z);
			glUniform3f(shader->Uniform(("spotLights[" + number + "].ambient").c_str()), m_spotLights[i]->m_ambient.x, m_spotLights[i]->m_ambient.y, m_spotLights[i]->m_ambient.z);
			glUniform3f(shader->Uniform(("spotLights[" + number + "].diffuse").c_str()), m_spotLights[i]->m_diffuse.x, m_spotLights[i]->m_diffuse.y, m_spotLights[i]->m_diffuse.z);
			glUniform3f(shader->Uniform(("spotLights[" + number + "].specular").c_str()), m_spotLights[i]->m_specular.x, m_spotLights[i]->m_specular.y, m_spotLights[i]->m_specular.z);
			glUniform1f(shader->Uniform(("spotLights[" + number + "].constant").c_str()), m_spotLights[i]->m_constant);
			glUniform1f(shader->Uniform(("spotLights[" + number + "].linear").c_str()), m_spotLights[i]->m_linear);
			glUniform1f(shader->Uniform(("spotLights[" + number + "].quadratic").c_str()), m_spotLights[i]->m_quadratic);
			glUniform1f(shader->Uniform(("spotLights[" + number + "].cutoff").c_str()), m_spotLights[i]->m_cutoff);
			glUniform1f(shader->Uniform(("spotLights[" + number + "].outerCutoff").c_str()), m_spotLights[i]->m_outerCutoff);
		}

		model->Draw(shader);
		shader->Unbind();
	}
}

//----------------------------------------------------------
void	BasicScene::CreateLights()
{
	// directionnal

	m_dirLight = new DirectionalLight();
	m_dirLight->m_ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	m_dirLight->m_diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
	m_dirLight->m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	m_dirLight->m_direction = glm::vec3(-0.2f, -1.0f, -0.3f);

	// points
	glm::vec3 pointLightPositions[] =
	{
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -4.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 pointLightColors[] =
	{
		glm::vec3(0.2f,  0.2f,  1.0f),
		glm::vec3(0.2f, 1.f, 0.2f),
		glm::vec3(1.0f,  0.2f, 0.2f),
		glm::vec3(1.f,  1.f, 1.f)
	};

	for (int i = 0; i < 4; i++)
	{
		PointLight* light = new PointLight();

		light->m_ambient = pointLightColors[i] * 0.2f; // glm::vec3(0.2f, 0.2f, 0.2f);
		light->m_diffuse = pointLightColors[i] * 0.5f; // glm::vec3(0.5f, 0.5f, 0.5f);
		light->m_specular = pointLightColors[i]; // glm::vec3(1.0f, 1.0f, 1.0f);
		light->Position() = pointLightPositions[i];
		light->m_constant = 1.f;
		light->m_linear = 0.09f;
		light->m_quadratic = 0.032f;

		m_pointLights.push_back(light);
	}


	for (int i = 0; i < 4; i++)
	{
		SpotLight* light = new SpotLight();

		float angle = 2 * PI * (i + 1) / 4.f;

		light->m_ambient = pointLightColors[i] * 0.2f; // glm::vec3(0.2f, 0.2f, 0.2f);
		light->m_diffuse = pointLightColors[i] * 0.5f; // glm::vec3(0.5f, 0.5f, 0.5f);
		light->m_specular = pointLightColors[i]; // glm::vec3(1.0f, 1.0f, 1.0f);
		light->Position() = glm::vec3(cos(angle), 3.f, sin(angle));
		light->m_direction = glm::vec3(0.f, -1.f, 0.f);
		light->m_constant = 1.f;
		light->m_linear = 0.09f;
		light->m_quadratic = 0.032f;
		light->m_cutoff = glm::cos(glm::radians(12.5f));
		light->m_outerCutoff = glm::cos(glm::radians(17.5f));

		m_spotLights.push_back(light);
	}
}

//----------------------------------------------------------
void	BasicScene::OnScrollMoved(float xoffset, float yoffset)
{
	super::OnScrollMoved(xoffset, yoffset);
}

//----------------------------------------------------------
void	BasicScene::OnKeyEvent(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		m_camera.SwitchMode();
	}

	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		m_sceneMode = ESceneMode::Normal;
	}

	if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
	{
		m_sceneMode = ESceneMode::Depth;
	}

	super::OnKeyEvent(key, scancode, action, mods);
}

