/*
 * glad.h — Minimal OpenGL 3.3 Core Profile loader header
 * Generated for CUIFramework. For full GLAD, visit https://glad.dav1d.de/
 */

#ifndef GLAD_H
#define GLAD_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------- Platform detection ----------------------------- */
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
    #define GLAD_PLATFORM_WINDOWS 1
#elif defined(__APPLE__)
    #define GLAD_PLATFORM_MACOS 1
#elif defined(__linux__)
    #define GLAD_PLATFORM_LINUX 1
#else
    #error "Unsupported platform"
#endif

/* ----------------------------- API export/import ----------------------------- */
#if defined(GLAD_PLATFORM_WINDOWS)
    #if defined(GLAD_BUILDING_DLL)
        #define GLAD_APIENTRY __declspec(dllexport)
    #elif defined(GLAD_DLL)
        #define GLAD_APIENTRY __declspec(dllimport)
    #else
        #define GLAD_APIENTRY
    #endif
#else
    #if defined(GLAD_BUILDING_DLL)
        #define GLAD_APIENTRY __attribute__((visibility("default")))
    #else
        #define GLAD_APIENTRY
    #endif
#endif

#ifndef GLAD_APIENTRY
    #define GLAD_APIENTRY
#endif

#ifndef APIENTRY
    #define APIENTRY GLAD_APIENTRY
#endif

/* -------------------------------- GL types -------------------------------- */
typedef void            GLvoid;
typedef unsigned int    GLenum;
typedef float           GLfloat;
typedef int             GLint;
typedef int             GLsizei;
typedef unsigned int    GLbitfield;
typedef double          GLdouble;
typedef unsigned int    GLuint;
typedef unsigned char   GLboolean;
typedef unsigned char   GLubyte;
typedef char            GLchar;
typedef GLint           GLintptr;
typedef GLint           GLsizeiptr;
typedef GLintptr        GLsync;
typedef unsigned int    GLbitfield;
typedef unsigned long long GLuint64;

/* ----------------------------- GL constants ----------------------------- */
#define GL_FALSE                          0
#define GL_TRUE                           1
#define GL_NONE                           0
#define GL_NO_ERROR                       0
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_QUADS                          0x0007

#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000

#define GL_BLEND                          0x0BE2
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_ZERO                           0
#define GL_ONE                            1

#define GL_DEPTH_TEST                     0x0B71
#define GL_CULL_FACE                      0x0B44
#define GL_SCISSOR_TEST                   0x0C11
#define GL_STENCIL_TEST                   0x0B90

#define GL_COLOR_ARRAY                    0x8076
#define GL_VERTEX_ARRAY                   0x8074

#define GL_TEXTURE_2D                     0x0DE1
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
#define GL_REPEAT                         0x2901
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_CLAMP_TO_BORDER                0x812D

#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3

#define GL_VIEWPORT                       0x0BA2

#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A

#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_GEOMETRY_SHADER                0x8DD9

#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_STREAM_DRAW                    0x88E0

#define GL_FLOAT                          0x1406
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_UNSIGNED_INT                   0x1405
#define GL_BYTE                           0x1400
#define GL_SHORT                          0x1402
#define GL_INT                            0x1404
#define GL_DOUBLE                         0x140A

#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C

#define GL_RED                            0x1903
#define GL_RG                             0x8227
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_ALPHA                          0x1906
#define GL_LUMINANCE                      0x1909
#define GL_LUMINANCE_ALPHA                0x190A

#define GL_PACK_ALIGNMENT                 0x0D05
#define GL_UNPACK_ALIGNMENT               0x0CF5

/* ----------------------------- Loader function pointer type ----------------------------- */
typedef void* (*GLADloadfunc)(const char* name);
typedef void  (*GLADglProc)(void);

/* ----------------------------- OpenGL 1.0 ----------------------------- */
typedef void  (APIENTRY *PFNGLCLEARPROC)(GLbitfield mask);
typedef void  (APIENTRY *PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void  (APIENTRY *PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef GLenum (APIENTRY *PFNGLGETERRORPROC)(void);
typedef const GLubyte* (APIENTRY *PFNGLGETSTRINGPROC)(GLenum name);
typedef void  (APIENTRY *PFNGLGETINTEGERVPROC)(GLenum pname, GLint* data);
typedef void  (APIENTRY *PFNGLENABLEPROC)(GLenum cap);
typedef void  (APIENTRY *PFNGLDISABLEPROC)(GLenum cap);
typedef void  (APIENTRY *PFNGLSCISSORPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void  (APIENTRY *PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
typedef void  (APIENTRY *PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
typedef void  (APIENTRY *PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);

/* ----------------------------- OpenGL 1.1 ----------------------------- */
typedef void  (APIENTRY *PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void  (APIENTRY *PFNGLGENTEXTURESPROC)(GLsizei n, GLuint* textures);
typedef void  (APIENTRY *PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint* textures);
typedef void  (APIENTRY *PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void  (APIENTRY *PFNGLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void  (APIENTRY *PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void  (APIENTRY *PFNGLTEXENVIPROC)(GLenum target, GLenum pname, GLint param);
typedef void  (APIENTRY *PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void* indices);
typedef void  (APIENTRY *PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);

/* ----------------------------- OpenGL 1.3 ----------------------------- */
typedef void  (APIENTRY *PFNGLACTIVETEXTUREPROC)(GLenum texture);

/* ----------------------------- OpenGL 1.5 ----------------------------- */
typedef void  (APIENTRY *PFNGLGENBUFFERSPROC)(GLsizei n, GLuint* buffers);
typedef void  (APIENTRY *PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void  (APIENTRY *PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint* buffers);
typedef void  (APIENTRY *PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void  (APIENTRY *PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);

/* ----------------------------- OpenGL 2.0 ----------------------------- */
typedef GLuint (APIENTRY *PFNGLCREATESHADERPROC)(GLenum type);
typedef void   (APIENTRY *PFNGLDELETESHADERPROC)(GLuint shader);
typedef void   (APIENTRY *PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
typedef void   (APIENTRY *PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void   (APIENTRY *PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint* params);
typedef void   (APIENTRY *PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef GLuint (APIENTRY *PFNGLCREATEPROGRAMPROC)(void);
typedef void   (APIENTRY *PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void   (APIENTRY *PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void   (APIENTRY *PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
typedef void   (APIENTRY *PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void   (APIENTRY *PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void   (APIENTRY *PFNGLVALIDATEPROGRAMPROC)(GLuint program);
typedef void   (APIENTRY *PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint* params);
typedef void   (APIENTRY *PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void   (APIENTRY *PFNGLGETACTIVEATTRIBPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef GLint  (APIENTRY *PFNGLGETATTRIBLOCATIONPROC)(GLuint program, const GLchar* name);
typedef void   (APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void   (APIENTRY *PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void   (APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef void   (APIENTRY *PFNGLGETACTIVEUNIFORMPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef GLint  (APIENTRY *PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar* name);
typedef void   (APIENTRY *PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void   (APIENTRY *PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void   (APIENTRY *PFNGLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
typedef void   (APIENTRY *PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void   (APIENTRY *PFNGLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void   (APIENTRY *PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

/* ----------------------------- OpenGL 3.0 ----------------------------- */
typedef void  (APIENTRY *PFNGLGENERATEMIPMAPROC)(GLenum target);
typedef void  (APIENTRY *PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint* framebuffers);
typedef void  (APIENTRY *PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint* framebuffers);
typedef void  (APIENTRY *PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
typedef GLenum (APIENTRY *PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
typedef void  (APIENTRY *PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void  (APIENTRY *PFNGLGENRENDERBUFFERSPROC)(GLsizei n, GLuint* renderbuffers);
typedef void  (APIENTRY *PFNGLDELETERENDERBUFFERSPROC)(GLsizei n, const GLuint* renderbuffers);
typedef void  (APIENTRY *PFNGLBINDRENDERBUFFERPROC)(GLenum target, GLuint renderbuffer);
typedef void  (APIENTRY *PFNGLRENDERBUFFERSTORAGEPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void  (APIENTRY *PFNGLFRAMEBUFFERRENDERBUFFERPROC)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void  (APIENTRY *PFNGLBLITFRAMEBUFFERPROC)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void  (APIENTRY *PFNGLENABLEIPROC)(GLenum target, GLuint index);
typedef void  (APIENTRY *PFNGLDISABLEIPROC)(GLenum target, GLuint index);
typedef const GLubyte* (APIENTRY *PFNGLGETSTRINGIPROC)(GLenum name, GLuint index);
typedef void  (APIENTRY *PFNGLVERTEXATTRIBIPOINTERPROC)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
typedef void  (APIENTRY *PFNGLGENERATEVERTEXARRAYPROC)(GLuint array);
typedef void  (APIENTRY *PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void  (APIENTRY *PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint* arrays);
typedef void  (APIENTRY *PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint* arrays);

/* ----------------------------- Loader ----------------------------- */
typedef struct {
    int GL_VERSION_1_0;
    int GL_VERSION_1_1;
    int GL_VERSION_1_2;
    int GL_VERSION_1_3;
    int GL_VERSION_1_4;
    int GL_VERSION_1_5;
    int GL_VERSION_2_0;
    int GL_VERSION_2_1;
    int GL_VERSION_3_0;
    int GL_VERSION_3_1;
    int GL_VERSION_3_2;
    int GL_VERSION_3_3;
} GLADVersion;

typedef struct {
    /* 1.0 */
    PFNGLCLEARPROC Clear;
    PFNGLCLEARCOLORPROC ClearColor;
    PFNGLVIEWPORTPROC Viewport;
    PFNGLGETERRORPROC GetError;
    PFNGLGETSTRINGPROC GetString;
    PFNGLGETINTEGERVPROC GetIntegerv;
    PFNGLENABLEPROC Enable;
    PFNGLDISABLEPROC Disable;
    PFNGLSCISSORPROC Scissor;
    PFNGLPIXELSTOREIPROC PixelStorei;
    PFNGLBLENDFUNCPROC BlendFunc;

    /* 1.1 */
    PFNGLBINDTEXTUREPROC BindTexture;
    PFNGLGENTEXTURESPROC GenTextures;
    PFNGLDELETETEXTURESPROC DeleteTextures;
    PFNGLTEXIMAGE2DPROC TexImage2D;
    PFNGLTEXSUBIMAGE2DPROC TexSubImage2D;
    PFNGLTEXPARAMETERIPROC TexParameteri;
    PFNGLDRAWELEMENTSPROC DrawElements;
    PFNGLDRAWARRAYSPROC DrawArrays;

    /* 1.3 */
    PFNGLACTIVETEXTUREPROC ActiveTexture;

    /* 1.5 */
    PFNGLGENBUFFERSPROC GenBuffers;
    PFNGLBINDBUFFERPROC BindBuffer;
    PFNGLDELETEBUFFERSPROC DeleteBuffers;
    PFNGLBUFFERDATAPROC BufferData;
    PFNGLBUFFERSUBDATAPROC BufferSubData;

    /* 2.0 */
    PFNGLCREATESHADERPROC CreateShader;
    PFNGLDELETESHADERPROC DeleteShader;
    PFNGLSHADERSOURCEPROC ShaderSource;
    PFNGLCOMPILESHADERPROC CompileShader;
    PFNGLGETSHADERIVPROC GetShaderiv;
    PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;
    PFNGLCREATEPROGRAMPROC CreateProgram;
    PFNGLDELETEPROGRAMPROC DeleteProgram;
    PFNGLATTACHSHADERPROC AttachShader;
    PFNGLDETACHSHADERPROC DetachShader;
    PFNGLLINKPROGRAMPROC LinkProgram;
    PFNGLUSEPROGRAMPROC UseProgram;
    PFNGLGETPROGRAMIVPROC GetProgramiv;
    PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;
    PFNGLGETATTRIBLOCATIONPROC GetAttribLocation;
    PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
    PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
    PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
    PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
    PFNGLUNIFORM1IPROC Uniform1i;
    PFNGLUNIFORM1FPROC Uniform1f;
    PFNGLUNIFORM2FPROC Uniform2f;
    PFNGLUNIFORM3FPROC Uniform3f;
    PFNGLUNIFORM4FPROC Uniform4f;
    PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;

    /* 3.0 */
    PFNGLGENERATEMIPMAPROC GenerateMipmap;
    PFNGLGENFRAMEBUFFERSPROC GenFramebuffers;
    PFNGLDELETEFRAMEBUFFERSPROC DeleteFramebuffers;
    PFNGLBINDFRAMEBUFFERPROC BindFramebuffer;
    PFNGLCHECKFRAMEBUFFERSTATUSPROC CheckFramebufferStatus;
    PFNGLFRAMEBUFFERTEXTURE2DPROC FramebufferTexture2D;
    PFNGLGENRENDERBUFFERSPROC GenRenderbuffers;
    PFNGLDELETERENDERBUFFERSPROC DeleteRenderbuffers;
    PFNGLBINDRENDERBUFFERPROC BindRenderbuffer;
    PFNGLRENDERBUFFERSTORAGEPROC RenderbufferStorage;
    PFNGLFRAMEBUFFERRENDERBUFFERPROC FramebufferRenderbuffer;
    PFNGLBLITFRAMEBUFFERPROC BlitFramebuffer;
    PFNGLVERTEXATTRIBIPOINTERPROC VertexAttribIPointer;
    PFNGLBINDVERTEXARRAYPROC BindVertexArray;
    PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
    PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
    PFNGLGETSTRINGIPROC GetStringi;
} GLADgl;

extern GLADVersion glad_version;
extern GLADgl glad_gl;

/* Convenience macros */
#define glClear             glad_gl.Clear
#define glClearColor        glad_gl.ClearColor
#define glViewport          glad_gl.Viewport
#define glGetError          glad_gl.GetError
#define glGetString         glad_gl.GetString
#define glGetIntegerv       glad_gl.GetIntegerv
#define glEnable            glad_gl.Enable
#define glDisable           glad_gl.Disable
#define glScissor           glad_gl.Scissor
#define glPixelStorei       glad_gl.PixelStorei
#define glBlendFunc         glad_gl.BlendFunc

#define glBindTexture       glad_gl.BindTexture
#define glGenTextures       glad_gl.GenTextures
#define glDeleteTextures    glad_gl.DeleteTextures
#define glTexImage2D        glad_gl.TexImage2D
#define glTexSubImage2D     glad_gl.TexSubImage2D
#define glTexParameteri     glad_gl.TexParameteri
#define glDrawElements      glad_gl.DrawElements
#define glDrawArrays        glad_gl.DrawArrays

#define glActiveTexture     glad_gl.ActiveTexture

#define glGenBuffers        glad_gl.GenBuffers
#define glBindBuffer        glad_gl.BindBuffer
#define glDeleteBuffers     glad_gl.DeleteBuffers
#define glBufferData        glad_gl.BufferData
#define glBufferSubData     glad_gl.BufferSubData

#define glCreateShader      glad_gl.CreateShader
#define glDeleteShader      glad_gl.DeleteShader
#define glShaderSource      glad_gl.ShaderSource
#define glCompileShader     glad_gl.CompileShader
#define glGetShaderiv       glad_gl.GetShaderiv
#define glGetShaderInfoLog  glad_gl.GetShaderInfoLog
#define glCreateProgram     glad_gl.CreateProgram
#define glDeleteProgram     glad_gl.DeleteProgram
#define glAttachShader      glad_gl.AttachShader
#define glDetachShader      glad_gl.DetachShader
#define glLinkProgram       glad_gl.LinkProgram
#define glUseProgram        glad_gl.UseProgram
#define glGetProgramiv      glad_gl.GetProgramiv
#define glGetProgramInfoLog glad_gl.GetProgramInfoLog
#define glGetAttribLocation glad_gl.GetAttribLocation
#define glEnableVertexAttribArray  glad_gl.EnableVertexAttribArray
#define glDisableVertexAttribArray glad_gl.DisableVertexAttribArray
#define glVertexAttribPointer     glad_gl.VertexAttribPointer
#define glGetUniformLocation      glad_gl.GetUniformLocation
#define glUniform1i        glad_gl.Uniform1i
#define glUniform1f        glad_gl.Uniform1f
#define glUniform2f        glad_gl.Uniform2f
#define glUniform3f        glad_gl.Uniform3f
#define glUniform4f        glad_gl.Uniform4f
#define glUniformMatrix4fv glad_gl.UniformMatrix4fv

#define glGenerateMipmap            glad_gl.GenerateMipmap
#define glGenFramebuffers           glad_gl.GenFramebuffers
#define glDeleteFramebuffers        glad_gl.DeleteFramebuffers
#define glBindFramebuffer           glad_gl.BindFramebuffer
#define glCheckFramebufferStatus    glad_gl.CheckFramebufferStatus
#define glFramebufferTexture2D      glad_gl.FramebufferTexture2D
#define glGenRenderbuffers          glad_gl.GenRenderbuffers
#define glDeleteRenderbuffers       glad_gl.DeleteRenderbuffers
#define glBindRenderbuffer          glad_gl.BindRenderbuffer
#define glRenderbufferStorage       glad_gl.RenderbufferStorage
#define glFramebufferRenderbuffer   glad_gl.FramebufferRenderbuffer
#define glBlitFramebuffer          glad_gl.BlitFramebuffer
#define glVertexAttribIPointer      glad_gl.VertexAttribIPointer
#define glBindVertexArray           glad_gl.BindVertexArray
#define glDeleteVertexArrays        glad_gl.DeleteVertexArrays
#define glGenVertexArrays           glad_gl.GenVertexArrays

int gladLoadGL(GLADloadfunc load);
int gladLoadGLLoader(GLADloadfunc load);

#ifdef __cplusplus
}
#endif

#endif /* GLAD_H */
