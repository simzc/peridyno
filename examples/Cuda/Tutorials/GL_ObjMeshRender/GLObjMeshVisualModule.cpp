#include "GLObjMeshVisualModule.h"
#include "ObjMesh.h"

#include <glad/glad.h>

#include <iostream>

bool loadImage(const char* path, dyno::CArray2D<dyno::Vec4f>& img);

IMPLEMENT_CLASS(GLObjMeshVisualModule)

bool GLObjMeshVisualModule::initializeGL()
{
	mVAO.create();

	mPositions.create(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);
	mNormals.create(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);
	mTexCoords.create(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);
	mInices.create(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

	mProgram.create();

	gl::Shader vshader, fshader;
	vshader.createFromFile(GL_VERTEX_SHADER, "shader/vert.glsl");
	fshader.createFromFile(GL_FRAGMENT_SHADER, "shader/frag.glsl");

	mProgram.attachShader(vshader);
	mProgram.attachShader(fshader);
	mProgram.link();

	vshader.release();
	fshader.release();

	mUniformBlock.create(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);

	return true;
}

void GLObjMeshVisualModule::updateGL()
{
	// need remap
	mPositions.mapGL();
	mNormals.mapGL();
	mTexCoords.mapGL();
	mInices.mapGL();

	mTexColor.updateGL();
}

void GLObjMeshVisualModule::releaseGL()
{
	mPositions.release();
	mNormals.release();
	mTexCoords.release();
	mInices.release();
	mTexColor.release();
	mProgram.release();

	mUniformBlock.release();
}

void GLObjMeshVisualModule::paintGL(const RenderParams& rparams)
{
	mProgram.use();

	// bind SSBO
	mPositions.bindBufferBase(0);
	mNormals.bindBufferBase(1);
	mTexCoords.bindBufferBase(2);

	// setup VAO
	mVAO.bind();
	mInices.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(0, 1, GL_INT, sizeof(Vec3i), (void*)0);	
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_INT, sizeof(Vec3i), (void*)4);
	glEnableVertexAttribArray(2);
	glVertexAttribIPointer(2, 1, GL_INT, sizeof(Vec3i), (void*)8);
	gl::glCheckError();

	// handle subroutine
	unsigned int subroutine;
	if (rparams.mode == GLRenderMode::COLOR) {
		subroutine = 0;
		// also load texture
		mTexColor.bind(GL_TEXTURE1);
	}
	else if (rparams.mode == GLRenderMode::SHADOW) {
		subroutine = 1;
	}
	else
	{
		return;
	}

	mUniformBlock.load((void*)&rparams, sizeof(RenderParams));
	mUniformBlock.bindBufferBase(0);

	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subroutine);
	
	if(mDrawCount > 0)
		glDrawArrays(GL_TRIANGLES, 0, mDrawCount);

	gl::glCheckError();
}


void GLObjMeshVisualModule::setColorTexture(const std::string& file)
{
	CArray2D<Vec4f> img;
	if (loadImage(file.c_str(), img)) {
		in_TexColor.assign(img);
	}
}

void GLObjMeshVisualModule::updateImpl()
{
	// copy to rendering buffer
	mPositions.load(in_Position.getData());
	mNormals.load(in_Normal.getData());
	mTexCoords.load(in_TexCoord.getData());
	mInices.load(in_Index.getData());

	// copy texture data...
	mTexColor.load(in_TexColor.getData());

	mDrawCount = in_Index.size();
}



