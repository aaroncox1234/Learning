#include "rxTexture.h"

namespace rx
{
    Texture::Texture( GLuint in_id, GLuint in_width, GLuint in_height) : id( in_id ),
                                                                         width( in_width ),
                                                                         height( in_height )
    {
    }

    Texture::~Texture()
    {
        glDeleteTextures( 1, &id );
    }

    void Texture::Draw( GLfloat x, GLfloat y ) const
    {
        glBindTexture( GL_TEXTURE_2D, id );
        glPushMatrix();
        glTranslatef( x, y, 0.0f );
        glBegin( GL_QUADS );
        glTexCoord2f( 0.f, 0.f ); glVertex2f( 0.f, 0.f );
        glTexCoord2f( 1.f, 0.f ); glVertex2f( width, 0.f );
        glTexCoord2f( 1.f, 1.f ); glVertex2f( width, height );
        glTexCoord2f( 0.f, 1.f ); glVertex2f( 0.f, height );
        glEnd();
        glPopMatrix();
        glBindTexture( GL_TEXTURE_2D, 0 );
    }
}

/*
THOUGHT: Just write these all as C functions. Let call manager the variables. i.e. thin wrapper

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

//
// program (holds multiple shaders)

GLuint mProgramID;

// load shader from file
loadShaderFromFile( std::string path, GLenum shaderType )
GLuint shaderID = 0;
std::string shaderString;
std::ifstream sourceFile( path.c_str() );
//Source file loaded
if( sourceFile ) {
//Get shader source
shaderString.assign( ( std::istreambuf_iterator< char >( sourceFile ) ), std::istreambuf_iterator< char >() );
//Create shader ID
shaderID = glCreateShader( shaderType );
//Set shader source
const GLchar* shaderSource = shaderString.c_str();
glShaderSource( shaderID, 1, (const GLchar**)&shaderSource, NULL );
//Compile shader source
glCompileShader( shaderID );
//Check shader for errors
GLint shaderCompiled = GL_FALSE;
glGetShaderiv( shaderID, GL_COMPILE_STATUS, &shaderCompiled );
if( shaderCompiled != GL_TRUE )
{ printf( "Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource );
  printShaderLog( shaderID );
  glDeleteShader( shaderID );
 shaderID = 0; } }
else { printf( "Unable to open file %s\n", path.c_str() ); }
return shaderID;

// load shader program
loadProgram() {
//Generate program
mProgramID = glCreateProgram();
//Load vertex shader
GLuint vertexShader = loadShaderFromFile( "30_loading_text_file_shaders/LPlainPolygonProgram2D.glvs", GL_VERTEX_SHADER );
//Check for errors
if( vertexShader == 0 ) {
glDeleteProgram( mProgramID );
mProgramID = 0;
return false; }
//Attach vertex shader to program
glAttachShader( mProgramID, vertexShader );
//Create fragment shader
GLuint fragmentShader = loadShaderFromFile( "30_loading_text_file_shaders/LPlainPolygonProgram2D.glfs", GL_FRAGMENT_SHADER );
//Check for errors
if( fragmentShader == 0 ) {
glDeleteProgram( mProgramID );
mProgramID = 0;
return false; }
//Attach fragment shader to program
glAttachShader( mProgramID, fragmentShader );
//Link program
glLinkProgram( mProgramID );
//Check for errors
GLint programSuccess = GL_TRUE;
glGetProgramiv( mProgramID, GL_LINK_STATUS, &programSuccess );
if( programSuccess != GL_TRUE ) {
printf( "Error linking program %d!\n", mProgramID );
printProgramLog( mProgramID );
glDeleteProgram( mProgramID );
mProgramID = 0;
return false; }
return true; }

// bind this program
bind
glUseProgram( mProgramID );
//Check for error
GLenum error = glGetError();
if( error != GL_NO_ERROR ) {
printf( "Error binding shader! %s\n", gluErrorString( error ) );
printProgramLog( mProgramID );
return false; }


//Initialize projection
mProjectionMatrix = glm::ortho<GLfloat>( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );
glUniformMatrix4fv( mProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr( mProjectionMatrix ) );
//Initialize modelview
mModelViewMatrix = glm::mat4();
gPlainPolygonProgram2D.updateModelView();
glUniformMatrix4fv( mModelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr( mModelViewMatrix ) );


glDeleteProgram( mProgramID );



// unbind this program
glUseProgram( NULL );

// print program log
//Make sure name is shader
if( glIsProgram( program ) ) {
//Program log length
int infoLogLength = 0;
int maxLength = infoLogLength;
//Get info string length
glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
//Allocate string
char* infoLog = new char[ maxLength ];
//Get info log
glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
if( infoLogLength > 0 ) {
//Print Log
printf( "%s\n", infoLog ); }
//Deallocate string
delete[] infoLog; }
else { printf( "Name %d is not a program\n", program ); }


//Make sure name is shader
if( glIsShader( shader ) ) {
//Shader log length
int infoLogLength = 0;
int maxLength = infoLogLength;
//Get info string length
glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
//Allocate string
char* infoLog = new char[ maxLength ];
//Get info log
glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
if( infoLogLength > 0 ) {
//Print Log
printf( "%s\n", infoLog ); }
//Deallocate string
delete[] infoLog; }
else { printf( "Name %d is not a shader\n", shader ); }

GLint mVertexPos2DLocation;
GLint mTexCoordLocation;
GLint mTextureColorLocation;
GLint mTextureUnitLocation;
glm::mat4 mProjectionMatrix;
GLint mProjectionMatrixLocation;
glm::mat4 mModelViewMatrix;
GLint mModelViewMatrixLocation;

mVertexPos2DLocation = 0;
mTexCoordLocation = 0;
mProjectionMatrixLocation = 0;
mModelViewMatrixLocation = 0;
mTextureColorLocation = 0;
mTextureUnitLocation = 0;

//Get variable locations
mVertexPos2DLocation = glGetAttribLocation( mProgramID, "LVertexPos2D" );
if( mVertexPos2DLocation == -1 )
{
	printf( "%s is not a valid glsl program variable!\n", "LVertexPos2D" );
}
mTexCoordLocation = glGetAttribLocation( mProgramID, "LTexCoord" );
if( mTexCoordLocation == -1 )
{
	printf( "%s is not a valid glsl program variable!\n", "LTexCoord" );
}
mTextureColorLocation = glGetUniformLocation( mProgramID, "LTextureColor" );
if( mTextureColorLocation == -1 )
{
	printf( "%s is not a valid glsl program variable!\n", "LTextureColor" );
}
mTextureUnitLocation = glGetUniformLocation( mProgramID, "LTextureUnit" );
if( mTextureUnitLocation == -1 )
{
	printf( "%s is not a valid glsl program variable!\n", "LTextureUnit" );
}
mProjectionMatrixLocation = glGetUniformLocation( mProgramID, "LProjectionMatrix" );
if( mProjectionMatrixLocation == -1 )
{
	printf( "%s is not a valid glsl program variable!\n", "LProjectionMatrix" );
}
mModelViewMatrixLocation = glGetUniformLocation( mProgramID, "LModelViewMatrix" );
if( mModelViewMatrixLocation == -1 )
{
	printf( "%s is not a valid glsl program variable!\n", "LModelViewMatrix" );
}


// Set variable (use to initialize and to change at runtime)
glVertexAttribPointer( mVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, stride, data );
glVertexAttribPointer( mTexCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, data );
glEnableVertexAttribArray( mVertexPos2DLocation );
glDisableVertexAttribArray( mVertexPos2DLocation );
glEnableVertexAttribArray( mTexCoordLocation );
glDisableVertexAttribArray( mTexCoordLocation );
mProjectionMatrix = matrix;
mModelViewMatrix = matrix;
mProjectionMatrix = matrix * mProjectionMatrix;
mModelViewMatrix = matrix * mModelViewMatrix;
glUniformMatrix4fv(	mProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr( mProjectionMatrix ) );
glUniformMatrix4fv(	mModelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr( mModelViewMatrix ) );
glUniform4f( mTextureColorLocation, color.r, color.g, color.b, color.a );
glUniform1i( mTextureUnitLocation, unit );


// Texture properties (hopefully not needed)
GLuint mTextureID;
GLuint* mPixels32;
GLubyte* mPixels8;
GLuint mPixelFormat;
GLuint mTextureWidth;
GLuint mTextureHeight;
GLuint mVBOID;
GLuint mIBOID;

GLenum DEFAULT_TEXTURE_WRAP = GL_REPEAT;

//
// Render

// Load render data
GLuint gVBO = NULL;
GLuint gIBO = NULL;
//VBO data
LVertexPos2D quadVertices[ 4 ];
GLuint indices[ 4 ];
//Set quad vertices
quadVertices[ 0 ].x = -50.f; quadVertices[ 0 ].y = -50.f;
quadVertices[ 1 ].x = 50.f; quadVertices[ 1 ].y = -50.f;
quadVertices[ 2 ].x = 50.f; quadVertices[ 2 ].y =
50.f; quadVertices[ 3 ].x = -50.f; quadVertices[ 3 ].y = 50.f;
//Set rendering indices
indices[ 0 ] = 0; indices[ 1 ] = 1; indices[ 2 ] = 2; indices[ 3 ] = 3;
//Create VBO
glGenBuffers( 1, &gVBO );
glBindBuffer( GL_ARRAY_BUFFER, gVBO );
glBufferData( GL_ARRAY_BUFFER, 4 * sizeof(LVertexPos2D), quadVertices, GL_STATIC_DRAW );
//Create IBO
glGenBuffers( 1, &gIBO );
glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indices, GL_STATIC_DRAW );

//Clear color buffer
glClear( GL_COLOR_BUFFER_BIT );
//Reset transformations
set and update model view to this: glm::translate<GLfloat>( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f )
//Solid cyan quad in the center
gPlainPolygonProgram2D.setColor( 0.f, 1.f, 1.f );  //nonsense tutorial function, just do glUniform4f( mPolygonColorLocation, r, g, b, a );
//Enable vertex arrays
glEnableClientState( GL_VERTEX_ARRAY );
//Set vertex data
glBindBuffer( GL_ARRAY_BUFFER, gVBO );
glVertexPointer( 2, GL_FLOAT, 0, NULL );
//Draw quad using vertex data and index data
glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );
//Disable vertex arrays
glDisableClientState( GL_VERTEX_ARRAY );

//
// Shader programs

blah.glvs
//Transformation Matrices
uniform mat4 LProjectionMatrix;
uniform mat4 LModelViewMatrix;
//Vertex position attribute
attribute vec2 LVertexPos2D;
//Texture coordinate attribute
attribute vec2 LTexCoord;
varying vec2 texCoord;
void main() {
//Process texCoord
texCoord = LTexCoord;
//Process vertex
gl_Position = LProjectionMatrix * LModelViewMatrix * vec4( LVertexPos2D.x, LVertexPos2D.y, 0.0, 1.0 ); }

blah.glfs
uniform vec4 LTextureColor;
uniform sampler2D LTextureUnit;
varying vec2 texCoord;
void main()
{
	gl_FragColor = texture( LTextureUnit, texCoord ) * LTextureColor;
}



todo:
lessons 33(maybe) and 34
get everything organized into functions and working
make sure alpha still works
slow text rendering with SDL
*/
