/*
 * glad.c — Minimal OpenGL 3.3 Core Profile loader implementation
 * Generated for CUIFramework.
 */

#include <glad/glad.h>

GLADVersion glad_version = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
GLADgl glad_gl = {0};

static GLADloadfunc _glad_loader = 0;

static void* load_gl_func(const char* name) {
    if (_glad_loader) {
        return _glad_loader(name);
    }
    return 0;
}

int gladLoadGL(GLADloadfunc load) {
    _glad_loader = load;
    if (!load) return 0;

    /* 1.0 */
    glad_gl.Clear = (PFNGLCLEARPROC)load_gl_func("glClear");
    glad_gl.ClearColor = (PFNGLCLEARCOLORPROC)load_gl_func("glClearColor");
    glad_gl.Viewport = (PFNGLVIEWPORTPROC)load_gl_func("glViewport");
    glad_gl.GetError = (PFNGLGETERRORPROC)load_gl_func("glGetError");
    glad_gl.GetString = (PFNGLGETSTRINGPROC)load_gl_func("glGetString");
    glad_gl.GetIntegerv = (PFNGLGETINTEGERVPROC)load_gl_func("glGetIntegerv");
    glad_gl.Enable = (PFNGLENABLEPROC)load_gl_func("glEnable");
    glad_gl.Disable = (PFNGLDISABLEPROC)load_gl_func("glDisable");
    glad_gl.Scissor = (PFNGLSCISSORPROC)load_gl_func("glScissor");
    glad_gl.PixelStorei = (PFNGLPIXELSTOREIPROC)load_gl_func("glPixelStorei");

    /* 1.1 */
    glad_gl.BindTexture = (PFNGLBINDTEXTUREPROC)load_gl_func("glBindTexture");
    glad_gl.GenTextures = (PFNGLGENTEXTURESPROC)load_gl_func("glGenTextures");
    glad_gl.DeleteTextures = (PFNGLDELETETEXTURESPROC)load_gl_func("glDeleteTextures");
    glad_gl.TexImage2D = (PFNGLTEXIMAGE2DPROC)load_gl_func("glTexImage2D");
    glad_gl.TexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC)load_gl_func("glTexSubImage2D");
    glad_gl.TexParameteri = (PFNGLTEXPARAMETERIPROC)load_gl_func("glTexParameteri");
    glad_gl.DrawElements = (PFNGLDRAWELEMENTSPROC)load_gl_func("glDrawElements");
    glad_gl.DrawArrays = (PFNGLDRAWARRAYSPROC)load_gl_func("glDrawArrays");

    /* 1.3 */
    glad_gl.ActiveTexture = (PFNGLACTIVETEXTUREPROC)load_gl_func("glActiveTexture");

    /* 1.5 */
    glad_gl.GenBuffers = (PFNGLGENBUFFERSPROC)load_gl_func("glGenBuffers");
    glad_gl.BindBuffer = (PFNGLBINDBUFFERPROC)load_gl_func("glBindBuffer");
    glad_gl.DeleteBuffers = (PFNGLDELETEBUFFERSPROC)load_gl_func("glDeleteBuffers");
    glad_gl.BufferData = (PFNGLBUFFERDATAPROC)load_gl_func("glBufferData");
    glad_gl.BufferSubData = (PFNGLBUFFERSUBDATAPROC)load_gl_func("glBufferSubData");

    /* 2.0 */
    glad_gl.CreateShader = (PFNGLCREATESHADERPROC)load_gl_func("glCreateShader");
    glad_gl.DeleteShader = (PFNGLDELETESHADERPROC)load_gl_func("glDeleteShader");
    glad_gl.ShaderSource = (PFNGLSHADERSOURCEPROC)load_gl_func("glShaderSource");
    glad_gl.CompileShader = (PFNGLCOMPILESHADERPROC)load_gl_func("glCompileShader");
    glad_gl.GetShaderiv = (PFNGLGETSHADERIVPROC)load_gl_func("glGetShaderiv");
    glad_gl.GetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)load_gl_func("glGetShaderInfoLog");
    glad_gl.CreateProgram = (PFNGLCREATEPROGRAMPROC)load_gl_func("glCreateProgram");
    glad_gl.DeleteProgram = (PFNGLDELETEPROGRAMPROC)load_gl_func("glDeleteProgram");
    glad_gl.AttachShader = (PFNGLATTACHSHADERPROC)load_gl_func("glAttachShader");
    glad_gl.DetachShader = (PFNGLDETACHSHADERPROC)load_gl_func("glDetachShader");
    glad_gl.LinkProgram = (PFNGLLINKPROGRAMPROC)load_gl_func("glLinkProgram");
    glad_gl.UseProgram = (PFNGLUSEPROGRAMPROC)load_gl_func("glUseProgram");
    glad_gl.GetProgramiv = (PFNGLGETPROGRAMIVPROC)load_gl_func("glGetProgramiv");
    glad_gl.GetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)load_gl_func("glGetProgramInfoLog");
    glad_gl.GetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)load_gl_func("glGetAttribLocation");
    glad_gl.EnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)load_gl_func("glEnableVertexAttribArray");
    glad_gl.DisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)load_gl_func("glDisableVertexAttribArray");
    glad_gl.VertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)load_gl_func("glVertexAttribPointer");
    glad_gl.GetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)load_gl_func("glGetUniformLocation");
    glad_gl.Uniform1i = (PFNGLUNIFORM1IPROC)load_gl_func("glUniform1i");
    glad_gl.Uniform1f = (PFNGLUNIFORM1FPROC)load_gl_func("glUniform1f");
    glad_gl.Uniform2f = (PFNGLUNIFORM2FPROC)load_gl_func("glUniform2f");
    glad_gl.Uniform3f = (PFNGLUNIFORM3FPROC)load_gl_func("glUniform3f");
    glad_gl.Uniform4f = (PFNGLUNIFORM4FPROC)load_gl_func("glUniform4f");
    glad_gl.UniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)load_gl_func("glUniformMatrix4fv");

    /* 3.0 */
    glad_gl.GenerateMipmap = (PFNGLGENERATEMIPMAPROC)load_gl_func("glGenerateMipmap");
    glad_gl.GenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)load_gl_func("glGenFramebuffers");
    glad_gl.DeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)load_gl_func("glDeleteFramebuffers");
    glad_gl.BindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)load_gl_func("glBindFramebuffer");
    glad_gl.CheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)load_gl_func("glCheckFramebufferStatus");
    glad_gl.FramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)load_gl_func("glFramebufferTexture2D");
    glad_gl.GenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)load_gl_func("glGenRenderbuffers");
    glad_gl.DeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)load_gl_func("glDeleteRenderbuffers");
    glad_gl.BindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)load_gl_func("glBindRenderbuffer");
    glad_gl.RenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)load_gl_func("glRenderbufferStorage");
    glad_gl.FramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)load_gl_func("glFramebufferRenderbuffer");
    glad_gl.BlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)load_gl_func("glBlitFramebuffer");
    glad_gl.VertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)load_gl_func("glVertexAttribIPointer");
    glad_gl.BindVertexArray = (PFNGLBINDVERTEXARRAYPROC)load_gl_func("glBindVertexArray");
    glad_gl.DeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)load_gl_func("glDeleteVertexArrays");
    glad_gl.GenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)load_gl_func("glGenVertexArrays");

    /* Detect version */
    if (glad_gl.GetString) {
        const char* version = (const char*)glad_gl.GetString(GL_VERSION);
        if (version) {
            int major = 0, minor = 0;
            if (sscanf(version, "%d.%d", &major, &minor) >= 1) {
                if (major >= 1) glad_version.GL_VERSION_1_0 = 1;
                if (major >= 1 && minor >= 1) glad_version.GL_VERSION_1_1 = 1;
                if (major >= 1 && minor >= 2) glad_version.GL_VERSION_1_2 = 1;
                if (major >= 1 && minor >= 3) glad_version.GL_VERSION_1_3 = 1;
                if (major >= 1 && minor >= 4) glad_version.GL_VERSION_1_4 = 1;
                if (major >= 1 && minor >= 5) glad_version.GL_VERSION_1_5 = 1;
                if (major >= 2) glad_version.GL_VERSION_2_0 = 1;
                if (major >= 2 && minor >= 1) glad_version.GL_VERSION_2_1 = 1;
                if (major >= 3) glad_version.GL_VERSION_3_0 = 1;
                if (major >= 3 && minor >= 1) glad_version.GL_VERSION_3_1 = 1;
                if (major >= 3 && minor >= 2) glad_version.GL_VERSION_3_2 = 1;
                if (major >= 3 && minor >= 3) glad_version.GL_VERSION_3_3 = 1;
            }
        }
    }

    return 1;
}

int gladLoadGLLoader(GLADloadfunc load) {
    return gladLoadGL(load);
}
