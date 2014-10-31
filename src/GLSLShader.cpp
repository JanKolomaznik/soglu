#include <GL/glew.h>
#include "soglu/GLSLShader.hpp"


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace soglu {

std::string
getShaderInfoLog(GLSLShaderId aShaderId)
{
	GLint errorLogLength = 0;
	GL_CHECKED_CALL(glGetShaderiv(aShaderId, GL_INFO_LOG_LENGTH, &errorLogLength));
	++errorLogLength;
	std::vector<char> errorMessage(errorLogLength);
	GL_CHECKED_CALL(glGetShaderInfoLog(aShaderId, errorLogLength, NULL, errorMessage.data()));
	return std::string(errorMessage.data());
}

std::string
getShaderProgramInfoLog(GLSLProgramId aProgramId)
{
	GLint errorLogLength = 0;
	GL_CHECKED_CALL(glGetProgramiv(aProgramId, GL_INFO_LOG_LENGTH, &errorLogLength));
	++errorLogLength;
	std::vector<char> errorMessage(errorLogLength);
	GL_CHECKED_CALL(glGetProgramInfoLog(aProgramId, errorLogLength, NULL, errorMessage.data()));
	return std::string(errorMessage.data());
}

void
checkForShaderCompileError(GLSLShaderId aShaderId)
{
	GLint compileStatus = GL_TRUE;
	GL_CHECKED_CALL(glGetShaderiv(aShaderId, GL_COMPILE_STATUS, &compileStatus));
	if (compileStatus == GL_TRUE) {
			return;
	}
	std::cout << getShaderInfoLog(aShaderId) << std::endl;
	SOGLU_THROW(EGLSLShaderCompileError());
}

void
checkForShaderProgramLinkError(GLSLProgramId aProgramId)
{
	GLint compileStatus = GL_TRUE;
	GL_CHECKED_CALL(glGetProgramiv(aProgramId, GL_LINK_STATUS, &compileStatus));
	if (compileStatus == GL_TRUE) {
			return;
	}
	std::cout << getShaderProgramInfoLog(aProgramId) << std::endl;
	SOGLU_THROW(EGLSLProgramLinkError());
}

void
checkForShaderProgramValidationError(GLSLProgramId aProgramId)
{
	GLint compileStatus = GL_TRUE;
	GL_CHECKED_CALL(glGetProgramiv(aProgramId, GL_VALIDATE_STATUS, &compileStatus));
	if (compileStatus == GL_TRUE) {
			return;
	}
	std::cout << getShaderProgramInfoLog(aProgramId) << std::endl;
	SOGLU_THROW(EGLSLProgramValidationError());
}

std::string loadFile(boost::filesystem::path filename)
{
	std::ifstream file;
	file.exceptions(std::ifstream::badbit);
	file.open(filename.string());

	std::string output;
	file.seekg(0, std::ios::end);
	output.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	output.assign(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
	return output;
}

GLSLProgram
createGLSLProgramFromVertexAndFragmentShader(const boost::filesystem::path &aVertexShader, const boost::filesystem::path &aFragmentShader)
{
	std::string vertexShader = loadFile(aVertexShader);
	std::string fragmentShader = loadFile(aFragmentShader);
	GLSLProgram program(true);
	program.attachShader(std::make_shared<GLSLVertexShader>(vertexShader));
	program.attachShader(std::make_shared<GLSLFragmentShader>(fragmentShader));
	program.link();
	program.validate();
	std::cout << "Program info " << getShaderProgramInfoLog(program.id()) << std::endl;
	return program;
}

std::vector<boost::filesystem::path>
getFilenamesFromPropertyTreeNode(const boost::property_tree::ptree &node, const boost::filesystem::path &aWorkingDirectory)
{
	std::vector<boost::filesystem::path>  filenames;
	for (const auto &child : node) {
		filenames.push_back(boost::filesystem::canonical(child.second.data(), aWorkingDirectory));
	}
	return filenames;
}

ShaderProgramSource
loadShaderProgramSource(const boost::filesystem::path &aConfigFile, const boost::filesystem::path &aWorkingDirectory)
{
	using boost::property_tree::ptree;
	using boost::property_tree::json_parser::read_json;
	ptree pt;

	std::cout << "Loading shader '" << aConfigFile << "'\nWorking directory: '" << aWorkingDirectory << "'\n";
	ShaderProgramSource config;

	config.name = aConfigFile.filename().string();

	read_json(aConfigFile.string(), pt);

	config.version = pt.get("version", 150);
	//pt.get();

	std::vector<boost::filesystem::path> vertexShaderSourceFiles =
			getFilenamesFromPropertyTreeNode(pt.get_child("sources.vertex_shader.files"), aWorkingDirectory);

	std::vector<boost::filesystem::path> fragmentShaderSourceFiles =
			getFilenamesFromPropertyTreeNode(pt.get_child("sources.fragment_shader.files"), aWorkingDirectory);

	std::cout << "Vertex shader sources:\n";
	for (const auto &filename : vertexShaderSourceFiles) {
		std::cout << "\t" << filename << std::endl;
		config.vertexShaderSources.push_back(soglu::loadFile(filename));
	}

	std::cout << "Fragment shader sources:\n";
	for (const auto &filename : fragmentShaderSourceFiles) {
		std::cout << "\t" << filename << std::endl;
		config.fragmentShaderSources.push_back(soglu::loadFile(filename));
	}
	return config;
}

GLSLProgram
createShaderProgramFromSources(const ShaderProgramSource &aSource, const std::string &aPrefix)
{
	std::cout << "Shader program: " << aSource.name << "\nPrefix:\n";
	std::cout << aPrefix << "\n";
	GLSLProgram program(true);
	{
		std::ostringstream vertexSource;
		vertexSource
				<< "#version " << aSource.version << std::endl
				<< aPrefix << std::endl;
		for (const auto &src : aSource.vertexShaderSources) {
			vertexSource << src << std::endl;
		}
		program.attachShader(std::make_shared<GLSLVertexShader>(vertexSource.str()));
	}

	{
		std::ostringstream fragmentSource;
		fragmentSource
				<< "#version " << aSource.version << std::endl
				<< aPrefix << std::endl;
		for (const auto &src : aSource.fragmentShaderSources) {
			fragmentSource << src << std::endl;
		}
		program.attachShader(std::make_shared<GLSLFragmentShader>(fragmentSource.str()));
	}
	program.link();
	program.validate();
	std::cout << "Program info " << program.id() << std::endl;
	std::cout << getShaderProgramInfoLog(program.id()) << std::endl;

	return program;
}

/*void testRender(GLSLProgram &aProgram, float aAspectRatio)
{
	static const GLfloat points[] = { -0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.0f, 1.0f };
	GLuint g_verticesVBO;
	GLuint g_vao;

	glm::mat4 projectionMatrix; // Store the projection matrix
	glm::mat4 viewMatrix; // Store the view matrix
	glm::mat4 modelMatrix; // Store the model matrix
	projectionMatrix = glm::perspective(60.0f, aAspectRatio, 0.1f, 100.f);

	viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f)); // Create our view matrix which will translate us back 5 units
	modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

	glm::mat4 modelViewProj = projectionMatrix * viewMatrix * modelMatrix;
	//aProgram.setUniform( "modelViewProj", modelViewProj);
	aProgram.bind();

	// Retrieve the vertex location in the program.
	int vertexLocation = aProgram.getAttributeLocation("vertex");
    glGenBuffers(1, &g_verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), (GLfloat*) points, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);

    // Bind the only used VBO in this example.
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glVertexAttribPointer(vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLocation);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &g_verticesVBO);
	glBindVertexArray(0);
    glDeleteVertexArrays(1, &g_vao);
}*/

}  // namespace soglu
