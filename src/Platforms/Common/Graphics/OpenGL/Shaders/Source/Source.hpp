
#pragma once

class ShaderSourceBase_ {
public:
	virtual ~ShaderSourceBase_() {}
	virtual const char * VertexSource() const = 0;
	virtual const int    VertexSourceLength() const = 0;
	virtual const char * FragmentSource() const = 0;
	virtual const int    FragmentSourceLength() const = 0;
};

class StaticShaderSource_
	:	public ShaderSourceBase_ {

	const char * vsrc;
	const int vsrc_length;
	const char * fsrc;
	const int fsrc_length;

public:

	StaticShaderSource_(
			const char * vsrc, const int vsrc_length,
			const char * fsrc, const int fsrc_length)
		:	vsrc(vsrc), vsrc_length(vsrc_length),
		 	fsrc(fsrc), fsrc_length(fsrc_length)
	{}

	const char * VertexSource() const { return vsrc; }
	const int    VertexSourceLength() const { return vsrc_length; }
	const char * FragmentSource() const { return fsrc; }
	const int    FragmentSourceLength() const { return fsrc_length; }
};

#ifndef FRAG_SHADER_PRECISION
#define FRAG_SHADER_PRECISION ""
#endif

#define KNIB_VERT_SRC\
		"attribute vec2 positionAttr;\n"\
		"attribute vec2 texcoordAttr;\n"\
		"uniform mat4 MVP;\n"\
		"\n"\
		"varying vec2 texcoordVar;\n"\
		"\n"\
		"void main(void)\n"\
		"{\n"\
		"	gl_Position = MVP * vec4(positionAttr.x,positionAttr.y,0.0,1.0);\n"\
		"	texcoordVar = texcoordAttr;\n"\
		"}\n";

#define KNIB_FRAG_SRC(channel)\
		FRAG_SHADER_PRECISION\
		"varying vec2 texcoordVar;\n"\
		"uniform sampler2D texSamp0;\n"\
		"uniform sampler2D texSamp1;\n"\
		"uniform sampler2D texSamp2;\n"\
		"\n"\
		"void main()\n"\
		"{\n"\
		"  vec3 ycbcr;\n"\
		"  vec3 rgb;\n"\
		"  ycbcr.x = texture2D( texSamp0, texcoordVar )." #channel ";\n"\
		"  ycbcr.y = texture2D( texSamp1, texcoordVar )." #channel "- 0.5;\n"\
		"  ycbcr.z = texture2D( texSamp2, texcoordVar )." #channel "- 0.5;\n"\
		"  rgb = mat3(  1,             1,       1,\n"\
		"	            0,       -.21482, 2.12798,\n"\
		"	            1.28033, -.38059,       0) * ycbcr;\n"\
		"  gl_FragColor = vec4(rgb.x,rgb.y,rgb.z,1.0);\n"\
		"}\n"

#define KNIB_FRAG_ALPHA_SRC(channel)\
		FRAG_SHADER_PRECISION\
		"varying vec2 texcoordVar;\n"\
		"uniform sampler2D texSamp0;\n"\
		"uniform sampler2D texSamp1;\n"\
		"uniform sampler2D texSamp2;\n"\
		"uniform sampler2D texSamp3;\n"\
		"\n"\
		"void main()\n"\
		"{\n"\
		"  vec3 ycbcr;\n"\
		"  vec3 rgb;\n"\
		"  ycbcr.x = texture2D( texSamp0, texcoordVar )." #channel ";\n"\
		"  ycbcr.y = texture2D( texSamp1, texcoordVar )." #channel "- 0.5;\n"\
		"  ycbcr.z = texture2D( texSamp2, texcoordVar )." #channel "- 0.5;\n"\
		"  rgb = mat3(  1,             1,       1,\n"\
		"	            0,       -.21482, 2.12798,\n"\
		"	            1.28033, -.38059,       0) * ycbcr;\n"\
		"  gl_FragColor = vec4(rgb.x,rgb.y,rgb.z, texture2D(texSamp3,texcoordVar)." #channel ");\n"\
		"}\n"

class ShaderSourceFactory_ {
public:

	enum e {
		BASIC,
		YUV420P,

		// For use with KnibVideo: https://github.com/chris-stones/KnibVideo */
		KNIB0_F0,
		KNIB0_F1,
		KNIB0_F2,
		KNIB0_F0_ALPHA,
		KNIB0_F1_ALPHA,
		KNIB0_F2_ALPHA,
		//
	};

	ShaderSourceBase_ * Get(e src) {

		switch(src) {
		default:
			return NULL;
		case BASIC:
			return Basic();
		case YUV420P:
			return Yuv420p();
		case KNIB0_F0:
			return KnibF0();
		case KNIB0_F1:
			return KnibF1();
		case KNIB0_F2:
			return KnibF2();
		case KNIB0_F0_ALPHA:
			return KnibAlphaF0();
		case KNIB0_F1_ALPHA:
			return KnibAlphaF1();
		case KNIB0_F2_ALPHA:
			return KnibAlphaF2();
		}
	}

private:

	ShaderSourceBase_ * Yuv420p() {

		static const char vsrc[] =
			"attribute vec2 positionAttr;\n"
			"attribute vec2 texcoordAttr;\n"
			"uniform mat4 MVP;\n"
			"\n"
			"varying vec2 texcoordVar;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"	gl_Position = MVP * vec4(positionAttr.x,positionAttr.y,0.0,1.0);\n"
			"	texcoordVar = texcoordAttr;\n"
			"}\n";

		static const char fsrc[] =
				FRAG_SHADER_PRECISION\
				"varying vec2 texcoordVar;\n"
				"uniform sampler2D texSampY;\n"
				"uniform sampler2D texSampCb;\n"
				"uniform sampler2D texSampCr;\n"
				"\n"
				"void main()\n"
				"{\n"
				"  vec3 ycbcr;\n"
				"  vec3 rgb;\n"
				"  ycbcr.x = texture2D( texSamp0, texcoordVar ).r;\n"
				"  ycbcr.y = texture2D( texSamp1, texcoordVar ).r - 0.5;\n"
				"  ycbcr.z = texture2D( texSamp2, texcoordVar ).r - 0.5;\n"
				"  rgb = mat3(      1,             1,       1,\n"
				"	            0,       -.21482, 2.12798,\n"
				"	            1.28033, -.38059,       0) * ycbcr;\n"
				"  gl_FragColor = vec4(rgb.x,rgb.y,rgb.z,1.0);\n"
				"}\n";

		return new StaticShaderSource_(vsrc, sizeof(vsrc), fsrc, sizeof(fsrc));
	}



	ShaderSourceBase_ * Basic() {

		static const char vsrc[] =
				"attribute vec2 positionAttr;\n"
				"attribute vec2 texcoordAttr;\n"
				"uniform mat4 MVP;\n"
				"\n"
				"varying vec2 texcoordVar;\n"
				"\n"
				"void main(void)\n"
				"{\n"
				"	gl_Position = MVP * vec4(positionAttr.x,positionAttr.y,0.0,1.0);\n"
				"	texcoordVar = texcoordAttr;\n"
				"}\n";

		static const char fsrc[] =
				FRAG_SHADER_PRECISION\
				"varying vec2 texcoordVar;\n"
				"uniform sampler2D texSamp;\n"
				"\n"
				"void main()\n"
				"{\n"
				"  gl_FragColor = texture2D(texSamp, texcoordVar);\n"
				"}\n";

		return new StaticShaderSource_(vsrc, sizeof(vsrc), fsrc, sizeof(fsrc));
	}

	ShaderSourceBase_ * KnibF0() {
		static const char vsrc[] = KNIB_VERT_SRC;
		static const char fsrc[] = KNIB_FRAG_SRC(r);
		return new StaticShaderSource_(vsrc, sizeof(vsrc), fsrc, sizeof(fsrc));
	}

	ShaderSourceBase_ * KnibF1() {
		static const char vsrc[] = KNIB_VERT_SRC;
		static const char fsrc[] = KNIB_FRAG_SRC(g);
		return new StaticShaderSource_(vsrc, sizeof(vsrc), fsrc, sizeof(fsrc));
	}

	ShaderSourceBase_ * KnibF2() {
		static const char vsrc[] = KNIB_VERT_SRC;
		static const char fsrc[] = KNIB_FRAG_SRC(b);
		return new StaticShaderSource_(vsrc, sizeof(vsrc), fsrc, sizeof(fsrc));
	}

	ShaderSourceBase_ * KnibAlphaF0() {
		static const char vsrc[] = KNIB_VERT_SRC;
		static const char fsrc[] = KNIB_FRAG_ALPHA_SRC(r);
		return new StaticShaderSource_(vsrc, sizeof(vsrc), fsrc, sizeof(fsrc));
	}

	ShaderSourceBase_ * KnibAlphaF1() {
		static const char vsrc[] = KNIB_VERT_SRC;
		static const char fsrc[] = KNIB_FRAG_ALPHA_SRC(g);
		return new StaticShaderSource_(vsrc, sizeof(vsrc), fsrc, sizeof(fsrc));
	}

	ShaderSourceBase_ * KnibAlphaF2() {
		static const char vsrc[] = KNIB_VERT_SRC;
		static const char fsrc[] = KNIB_FRAG_ALPHA_SRC(b);
		return new StaticShaderSource_(vsrc, sizeof(vsrc), fsrc, sizeof(fsrc));
	}
};


