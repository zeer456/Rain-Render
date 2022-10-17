#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#include "GL/glew.h"
#include "GL/gl.h"


template class __declspec(dllexport) std::map<std::string, int>;

class  ShaderProgram
{
private:
	
	// DEBUG Flag
	static const bool DEBUG = true;

	// Enum shader and shader program for log
	enum ObjectType
	{
		SHADER, PROGRAM
	};

	// Shader program & Shader Ids
	GLuint programId;

	GLuint vertexShaderId;
	GLuint fragmentShaderId;
	GLuint geometryShaderId;
	GLuint tessControlShaderId;
	GLuint tessEvalShaderId;

#pragma warning( disable: 4251 )
	std::string vertexShaderFileName;
	std::string fragmentShaderFileName;
#pragma warning( default: 4251 )

	// Number of shaders attached to the shader program
	GLuint shaderCount;

	std::map<std::string, int> attributeMap;

	std::map<std::string, int> uniformMap;

	std::vector<std::string> feedbackMap;

	std::map<std::string, int> subroutineMap;

	bool initialised;

	GLuint compileShader(std::string shaderSource, GLenum shaderType)
	{
		std::string shaderTypeString;
		std::string fileName;
		switch (shaderType)
		{
		case GL_VERTEX_SHADER:
			shaderTypeString = "GL_VERTEX_SHADER";
			fileName = vertexShaderFileName;
			break;
		case GL_FRAGMENT_SHADER:  
			shaderTypeString = "GL_FRAGMENT_SHADER";
			fileName = fragmentShaderFileName;
			break;
		case GL_GEOMETRY_SHADER:
			shaderTypeString = "GL_GEOMETRY_SHADER";
			break;

		case GL_TESS_CONTROL_SHADER:
			shaderTypeString = "GL_TESS_CONTROL_SHADER";
			break;

		case GL_TESS_EVALUATION_SHADER:
			shaderTypeString = "GL_TESS_EVALUATION_SHADER";
			break;

		default:
			throw std::runtime_error("Bad shader type enum in compileShader.");
			break;
		}

		GLuint shaderId = glCreateShader(shaderType);
		if (shaderId == 0)
		{
			throw std::runtime_error("Could not create shader of type " + shaderTypeString + ": " + getInfoLog(ObjectType::SHADER, shaderId));
		}

		const char *shaderSourceChars = shaderSource.c_str();

		glShaderSource(shaderId, 1, &shaderSourceChars, NULL);

		glCompileShader(shaderId);

		GLint shaderStatus;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderStatus);
		if (shaderStatus == GL_FALSE)
		{
			std::cout <<  shaderTypeString  << " compilation failed: " << getInfoLog(ObjectType::SHADER, shaderId) << std::endl;
		}
		else
		{
			if (DEBUG)
			{
				std::cout << shaderTypeString << " shader compilation successful. : " << fileName << std::endl;
			}
		}

		return shaderId;
	}


	void old_func(char** carray, std::size_t size)
	{
		for (std::size_t i(0); i < size; ++i)
			std::cout << carray[i] << '\n';
	}

	void initialise(std::string vertexShaderSource, std::string fragmentShaderSource, std::string geometryShaderSource = 0, std::string tessControlShaderSource = 0, std::string tessEvalShaderSource = 0)
	{
		vertexShaderId = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
	
		fragmentShaderId = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

		if (geometryShaderSource.length() > 0)
			geometryShaderId = compileShader(geometryShaderSource, GL_GEOMETRY_SHADER);

		if (tessControlShaderSource.length() > 0)
			tessControlShaderId = compileShader(tessControlShaderSource, GL_TESS_CONTROL_SHADER);

		if (tessEvalShaderSource.length() > 0)
			tessEvalShaderId = compileShader(tessEvalShaderSource, GL_TESS_EVALUATION_SHADER);

		glAttachShader(programId, vertexShaderId);
		glAttachShader(programId, fragmentShaderId);

		if (geometryShaderSource.length() > 0)
			glAttachShader(programId, geometryShaderId);
		
		if (tessControlShaderSource.length() > 0)
			glAttachShader(programId, tessControlShaderId);

		if (tessEvalShaderSource.length() > 0)
			glAttachShader(programId, tessEvalShaderId);

		if (feedbackMap.size() > 0) {
			std::vector<char*> cstrings{};
			for (auto& string : feedbackMap)
				cstrings.push_back(&string.front());

			old_func(cstrings.data(), cstrings.size());
			glTransformFeedbackVaryings(programId, feedbackMap.size(), cstrings.data(), GL_SEPARATE_ATTRIBS);

		}

		glLinkProgram(programId);

		glDetachShader(programId, vertexShaderId);
		glDetachShader(programId, fragmentShaderId);

		if (geometryShaderSource.length() > 0)
			glDetachShader(programId, geometryShaderId);

		if (tessControlShaderSource.length() > 0)
			glDetachShader(programId, tessControlShaderId);

		if (tessEvalShaderSource.length() > 0)
			glDetachShader(programId, tessEvalShaderId);

		GLint programLinkSuccess = GL_FALSE;
		glGetProgramiv(programId, GL_LINK_STATUS, &programLinkSuccess);
		if (programLinkSuccess == GL_TRUE)
		{
			if (DEBUG)
			{
				std::cout << "Shader program link successful." << std::endl;
			}
		}
		else
		{
			std::cout << "Shader program link failed: "  << getInfoLog(ObjectType::PROGRAM, programId) << std::endl;
		}

		glValidateProgram(programId);

		GLint programValidatationStatus;
		glGetProgramiv(programId, GL_VALIDATE_STATUS, &programValidatationStatus);
		if (programValidatationStatus == GL_TRUE)
		{
			if (DEBUG)
			{
				std::cout << "Shader program validation successful." << std::endl;
			}
		}
		else
		{
			 std::cout << "Shader program validation failed: "  << getInfoLog(ObjectType::PROGRAM, programId) << std::endl;
		}

		initialised = true;
	}

	std::string loadShaderFromFile(const std::string filename)
	{
		if (filename.size() == 0) return 0;

		std::ifstream file(filename.c_str());

		if (!file.good())
		{
			std::cout << "Failed to open file: "  <<  filename << std::endl;
		}

		std::stringstream stream;

		stream << file.rdbuf();

		file.close();

		return stream.str();
	}

	std::string getInfoLog(ObjectType type, int id)
	{
		GLint infoLogLength;
		if (type == ObjectType::SHADER)
		{
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
		}
		else
		{
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
		}

		GLchar *infoLog = new GLchar[infoLogLength + 1];
		if (type == ObjectType::SHADER)
		{
			glGetShaderInfoLog(id, infoLogLength, NULL, infoLog);
		}
		else
		{
			glGetProgramInfoLog(id, infoLogLength, NULL, infoLog);
		}

		std::string infoLogString(infoLog);

		delete[] infoLog;

		return infoLogString;
	}

public:
	ShaderProgram()
	{
		initialised = false;

		programId = glCreateProgram();
		glUseProgram(programId);

		shaderCount = 0;
	}

	~ShaderProgram()
	{
		glDeleteProgram(programId);
	}

	void initFromFiles(std::string vertexShaderFilename, std::string fragmentShaderFilename, std::string geometryShaderFilename = "", std::string tessControlShaderFilename = "", std::string tessEvalShaderFilename = "")
	{
		std::string vertexShaderSource = loadShaderFromFile(vertexShaderFilename);
		vertexShaderFileName = vertexShaderFilename;
		std::string fragmentShaderSource = loadShaderFromFile(fragmentShaderFilename);
		fragmentShaderFileName = fragmentShaderFilename;

		std::string geometryShaderSource;
		std::string tessControlShaderSource;
		std::string tessEvalShaderSource;

		if (geometryShaderFilename.size() > 0)
			geometryShaderSource = loadShaderFromFile(geometryShaderFilename);

		if (tessControlShaderFilename.size() > 0)
			tessControlShaderSource = loadShaderFromFile(tessControlShaderFilename);

		if (tessEvalShaderFilename.size() > 0)
			tessEvalShaderSource = loadShaderFromFile(tessEvalShaderFilename);

		initialise(vertexShaderSource, fragmentShaderSource, geometryShaderSource, tessControlShaderSource, tessEvalShaderSource);
	}

	void initFromStrings(std::string vertexShaderSource, std::string fragmentShaderSource, std::string geometryShaderSource = "", std::string tessControlShaderSource = "", std::string tessEvalShaderSource = "")
	{
		initialise(vertexShaderSource, fragmentShaderSource, geometryShaderSource, tessControlShaderSource, tessEvalShaderSource);
	}

	inline void use()
	{
		if (initialised)
		{
			glUseProgram(programId);
		}
		else
		{
			std::string msg = "Shader program " + programId;
			msg += " not initialised - aborting.";
			throw std::runtime_error(msg);
		}
	}

	inline void disable()
	{
		glUseProgram(0);
	}

	GLuint attribute(const std::string attributeName)
	{
		static std::map<std::string, int>::const_iterator attributeIter;

		attributeIter = attributeMap.find(attributeName);

		if (attributeIter == attributeMap.end())
		{
			std::cout << "Could not find attribute in shader program: "  <<  attributeName;
		}

		return attributeMap[attributeName];
	}

	GLuint uniform(const std::string uniformName)
	{
		static std::map<std::string, int>::const_iterator uniformIter;

		uniformIter = uniformMap.find(uniformName);

		if (uniformIter == uniformMap.end())
		{
			std::cout << "Could not find uniform in shader program: " <<  uniformName << std::endl;
		}

		return uniformMap[uniformName];
	}

	GLuint subroutine(const std::string uniformName)
	{
		static std::map<std::string, int>::const_iterator uniformIter;

		uniformIter = subroutineMap.find(uniformName);

		if (uniformIter == subroutineMap.end())
		{
			std::cout << "Could not find uniform in shader program: " << uniformName << std::endl;
		}

		return subroutineMap[uniformName];

	}

	int addAttribute(const std::string attributeName)
	{
		attributeMap[attributeName] = glGetAttribLocation(programId, attributeName.c_str());

		if (attributeMap[attributeName] == -1)
		{
			std::cout << "Could not add attribute: "  << attributeName  << " - location returned -1." << std::endl;
		}
		else
		{
			if (DEBUG)
			{
				std::cout << "Attribute " << attributeName << " bound to location: " << attributeMap[attributeName] << std::endl;
			}
		}

		return attributeMap[attributeName];
	}

	void addFeedBackOut(const std::string attributeName)
	{
		feedbackMap.push_back(attributeName);
	}

	int addUniform(const std::string uniformName)
	{
		uniformMap[uniformName] = glGetUniformLocation(programId, uniformName.c_str());

		if (uniformMap[uniformName] == -1)
		{
			std::cout <<  "Could not add uniform: "  << uniformName  <<  " - location returned -1." << std::endl;
			
		}
		else
		{
			if (DEBUG)
			{
				std::cout << "Uniform " << uniformName << " bound to location: " << uniformMap[uniformName] << std::endl;
			}
		}
		return uniformMap[uniformName];
	}


	int addSubroutine(GLenum shadertype, const std::string uniformName)
	{
		subroutineMap[uniformName] = glGetSubroutineIndex(programId, shadertype, uniformName.c_str());

		if (subroutineMap[uniformName] == -1)
		{
			std::cout << "Could not add uniform: " << uniformName << " - location returned -1." << std::endl;

		}
		else
		{
			if (DEBUG)
			{
				std::cout << "Uniform " << uniformName << " bound to location: " << subroutineMap[uniformName] << std::endl;
			}
		}
		return subroutineMap[uniformName];
	}



	GLuint getID() 
	{
		return programId;
	}

};
#endif