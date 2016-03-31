
#pragma once

#include "MeshComponent.h"
#include "GeometryComponent.h"
#include "../Object.h"

namespace byd

{

typedef std::shared_ptr<class MeshRendererComponent> MeshRendererComponentSharedPtr;

class MeshRendererComponent : public Component
{
public:

	MeshRendererComponent();
	virtual ~MeshRendererComponent() noexcept;

	void setMeshObject(ObjectWeakPtr m)
	{
		this->meshObject = m;
	}

	void Render()
	{	
		auto mesh = this->meshObject.lock();

		if (!mesh)
		{
			// nothing todo
			return;
		}

		auto geometry = mesh->FindComponent<GeometryComponent>();

		if (!geometry)
		{
			return;
		}	

		// !!! TODO: move shit below to proper components! and error checking!

		GLint currentProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

		GLint currentBuffer;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBuffer);

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		static const char * vTxt = R"qwertyuiop(
			attribute vec3 pos;
			attribute vec3 color;

			varying vec3 colorOut;

			void main(void)
			{
				colorOut = color;
				gl_Position = vec4(pos, 1.0);		
			}
			)qwertyuiop";
		glShaderSource(vertexShader, 1, &vTxt, 0);
		ASSERT(glGetError() == GL_NO_ERROR)
		glCompileShader(vertexShader); //TODO: получается, ты компилируешь шейдеры каждый кадр тут... <DR> see my comment above 45 line
		ASSERT(glGetError() == GL_NO_ERROR)

		GLint cs;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &cs);
		TRACE (<< " cs " << cs << std::endl)

		GLchar m[256];
		glGetShaderInfoLog(vertexShader, sizeof(m), 0, &m[0]);
		TRACE (<< m << std::endl)

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		static const char *fTxt = R"qwertyuiop(	

			varying vec3 colorOut;

			void main(void)
			{
				gl_FragColor = vec4(colorOut, 1.0);
			}
			)qwertyuiop";
		ASSERT(glGetError() == GL_NO_ERROR)
		glShaderSource(fragmentShader, 1, &fTxt, 0);
		ASSERT(glGetError() == GL_NO_ERROR)
		glCompileShader(fragmentShader);
		ASSERT(glGetError() == GL_NO_ERROR)


		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShader);
		ASSERT(glGetError() == GL_NO_ERROR)
		glAttachShader(program, fragmentShader);
		ASSERT(glGetError() == GL_NO_ERROR)
		glLinkProgram(program);
		ASSERT(glGetError() == GL_NO_ERROR)

		glUseProgram(program);

		GLint pos = glGetAttribLocation(program, "pos");
		TRACE (<< glGetError() << std::endl)
		ASSERT(glGetError() == GL_NO_ERROR)

		GLint color = glGetAttribLocation(program, "color");
		TRACE (<< glGetError() << std::endl)
		ASSERT(glGetError() == GL_NO_ERROR)

		glBindBuffer(GL_ARRAY_BUFFER, geometry->VertexBufferId());

		glEnableVertexAttribArray(pos);
		ASSERT(glGetError() == GL_NO_ERROR)
		glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		ASSERT(glGetError() == GL_NO_ERROR)

		glEnableVertexAttribArray(color);
		glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<GLvoid *>(sizeof(float) * 3));
		ASSERT(glGetError() == GL_NO_ERROR)

		ASSERT(glGetError() == GL_NO_ERROR)	
		TRACE (<< "idx count " << geometry->IndexCount() << std::endl)

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->IndexBufferId());
		glDrawElements(GL_TRIANGLES, geometry->IndexCount(), GL_UNSIGNED_INT, 0);
		ASSERT(glGetError() == GL_NO_ERROR)

		// update GL pipeline or something
		geometry->traceGL();

		glDisableVertexAttribArray(pos);
		glDisableVertexAttribArray(color);

		//TODO: зачем делать это я так и не понял...<DR> see my comment above 45 line
		glUseProgram(currentProgram);
		glBindBuffer(GL_ARRAY_BUFFER, currentBuffer);
	}

private:
	ObjectWeakPtr meshObject;
	// TODO:
	// Effects, Program(shaders) etc.
};

}
