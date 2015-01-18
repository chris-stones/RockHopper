
#pragma once

#include <memory>

class ShaderFactory_ {

	static GLuint compile_shader( GLenum shaderType, const GLchar * src, int src_len ) {

		GLuint shader = glCreateShader( shaderType );

		glShaderSource( shader, 1, &src, &src_len );
		glCompileShader(shader);

		GLint param = GL_FALSE;

		glGetShaderiv(shader,GL_COMPILE_STATUS, &param);

		if( param != GL_TRUE ) {

			GLint logLength = 0;
			GLint rlogLength = 0;
			GLchar * log = NULL;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			log = (GLchar*)alloca( logLength+1 );
			if(log) {
				memset(log,0,logLength+1);
				glGetShaderInfoLog(shader, logLength, &rlogLength, log);
				printf("RH %s\n",log); // TODO: real logging!
			}

			glDeleteShader(shader);
			shader = 0;
		}

		return shader;
	}

	static GLint link_program(GLuint program, GLuint vshader, GLuint fshader) {

		GLint linkparam = GL_FALSE;
		GLint validparam = GL_FALSE;

		glAttachShader( program, vshader );
		glAttachShader( program, fshader );
		glLinkProgram( program );
		glGetProgramiv( program, GL_LINK_STATUS, &linkparam );
		glValidateProgram( program );
		glGetProgramiv( program, GL_LINK_STATUS, &validparam );

		if(linkparam == GL_TRUE && validparam == GL_TRUE)
			return GL_TRUE;

		return GL_FALSE;
	}

	static GLuint create_simple_program(const GLchar * vsource, int vsource_size, const GLchar *fsource, int fsource_size ) {

		GLuint program = 0;
		GLuint vshader = 0;
		GLuint fshader = 0;
		GLint  linkedparam = GL_FALSE;

		GL_ERROR();

		program = glCreateProgram();
		vshader = compile_shader( GL_VERTEX_SHADER,   vsource, vsource_size );
		fshader = compile_shader( GL_FRAGMENT_SHADER, fsource, fsource_size );

		if(program && vshader && fshader)
			linkedparam = link_program( program, vshader, fshader);

		if( vshader )
			glDeleteShader( vshader );
		if( fshader )
			glDeleteShader( fshader );

		if(program && (linkedparam != GL_TRUE))
			glDeleteProgram(program);

		if (linkedparam != GL_TRUE)
			program = 0;

		GL_ERROR();

		return program;
	}

	static GLuint CreateProgram( ShaderSourceFactory_::e source ) {

		ShaderSourceFactory_ ssf;

		std::unique_ptr<ShaderSourceBase_> ss =
				std::unique_ptr<ShaderSourceBase_>(ssf.Get(source));

		GLuint p = create_simple_program(
					ss->VertexSource(),
					ss->VertexSourceLength(),
					ss->FragmentSource(),
					ss->FragmentSourceLength());

		return p;
	}

public:

	YUV420PShader_ * CreateYUV420PShader() {

		return new YUV420PShader_(
			CreateProgram( ShaderSourceFactory_::YUV420P ));
	}

	BasicShader_* CreateBasicShader() {

		return new BasicShader_(
			CreateProgram( ShaderSourceFactory_::BASIC ));
	}

	KnibShader_* CreateKnibShader(int f, bool alpha) {

		if(alpha) {
			switch(f) {
			default:
			case 0: return new KnibShader_( CreateProgram( ShaderSourceFactory_::KNIB0_F0_ALPHA ), true);
			case 1: return new KnibShader_( CreateProgram( ShaderSourceFactory_::KNIB0_F1_ALPHA ), true);
			case 2: return new KnibShader_( CreateProgram( ShaderSourceFactory_::KNIB0_F2_ALPHA ), true);
			}
		}
		else
		{
			switch(f) {
			default:
			case 0: return new KnibShader_( CreateProgram( ShaderSourceFactory_::KNIB0_F0 ), false);
			case 1: return new KnibShader_( CreateProgram( ShaderSourceFactory_::KNIB0_F1 ), false);
			case 2: return new KnibShader_( CreateProgram( ShaderSourceFactory_::KNIB0_F2 ), false);
			}
		}
	}
};

