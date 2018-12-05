/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>

#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>

#include <GLW/API.h>

namespace glw
{

  enum class Wrapping
  {
    REPEAT = uint32_t(gl::GLenum::GL_REPEAT),
    MIRROR_REPEAT = uint32_t(gl::GLenum::GL_MIRRORED_REPEAT),
    CLAMP = uint32_t(gl::GLenum::GL_CLAMP_TO_EDGE),
    BORDER_COLOR = uint32_t(gl::GLenum::GL_CLAMP_TO_BORDER)
  };


  /*!
    Texture filtering type. Defines how colors are pulled from texture image
     when picking from between multiple texel points.
  */
  enum class Filtering
  {
    NEAREST = uint32_t(gl::GLenum::GL_NEAREST),
    LINEAR = uint32_t(gl::GLenum::GL_LINEAR),
    NEAREST_BEST_MIP = uint32_t(gl::GLenum::GL_NEAREST_MIPMAP_NEAREST),
    LINEAR_BEST_MIP = uint32_t(gl::GLenum::GL_LINEAR_MIPMAP_NEAREST),
    NEAREST_INTER_MIP = uint32_t(gl::GLenum::GL_NEAREST_MIPMAP_LINEAR),
    LINEAR_INTER_MIP = uint32_t(gl::GLenum::GL_LINEAR_MIPMAP_LINEAR)
  };



  enum class ColorType
  {
    UINT8_N = 0,	//! range < 0.0, 1.0> internally represented as 8 bit unsigned integral number
    INT8_N,			  //! range <-1.0, 1.0> internally represented as 8 bit integral number
    UINT16_N,		  //! range < 0.0, 1.0> internally represented as 16 bit unsigned integral number
    INT16_N,		  //! range <-1.0, 1.0> internally represented as 16 bit integral number
    UINT8,			  //! true 8 bit unsigned integral number
    INT8,			    //! true 8 bit integral number
    UINT16,			  //! true 16 bit unsigned integral number
    INT16,			  //! true 16 bit integral number
    UINT32,			  //! true 32 bit unsigned integral number
    INT32,			  //! true 32 bit integral number
    FLOAT16,		  //! 16 bit floating point number
    FLOAT32			  //! 32 bit floating point number
  };

  //! Creates OpenGL color format from desired data type per channel, and desired number of channels.
  GLW_API gl::GLenum ColorFormat(const ColorType color_data_type, const uint8_t channels);



  enum class DepthDataFormat
  {
    UINT16_N = uint32_t(gl::GLenum::GL_DEPTH_COMPONENT16),  //! range < 0.0, 1.0> internally represented as 16 bit unsigned integral number
    UINT24_N = uint32_t(gl::GLenum::GL_DEPTH_COMPONENT24),  //! range < 0.0, 1.0> internally represented as 24 bit unsigned integral number
    FLOAT32 = uint32_t(gl::GLenum::GL_DEPTH_COMPONENT32F)   //! 32 bit floating point number
  };



  using PTexture = std::shared_ptr<class Texture>;



  //! Basic texture type.
  class GLW_API Texture
  {
  public:

    virtual ~Texture();
    void Initialize(const gl::GLenum internal_format);

    void Enable();
    void Disable();
    void Unbind();

    void Bind();
    void Bind(const uint8_t binding_unit);

    gl::GLuint GetID();

    gl::GLenum GetInternalFormat();

    void SetFiltering(const Filtering mag_min);
    void SetFiltering(const Filtering mag, const Filtering min);

    void SetWrapping(const Wrapping wrap_st);
    void SetWrapping(const Wrapping wrap_s, const Wrapping wrap_t);

    void SetParameteri(const gl::GLenum pname, const gl::GLint param);
    void SetParameterfv(const gl::GLenum pname, const gl::GLfloat *param);

  protected:
    virtual gl::GLenum GetTarget() = 0;
    virtual void ReserveMemory() = 0;

  private:
    void GenerateId();
    gl::GLenum internal_format_ = gl::GLenum::GL_RGBA8;
    gl::GLuint texture_id_ = 0;
  };



  using PTexture2D = std::shared_ptr<class Texture2D>;



  //! Two dimensional texture. Used mostly to store color images or results of internal rendering.
  class GLW_API Texture2D : public Texture
  {
  public:

    static PTexture2D Create(const uint32_t width, const uint32_t height, const gl::GLenum internal_format = gl::GLenum::GL_RGBA8);
    static PTexture2D CreateLayered(const uint32_t width, const uint32_t height, const uint32_t layer_count, const gl::GLenum internal_format = gl::GLenum::GL_RGBA8);
    static PTexture2D CreateFromFile(const std::string &image_file_name, const bool load_as_float = false);

  public:
    //! Creates a singl::gle 2D texture.
    Texture2D(const uint32_t width, const uint32_t height, const gl::GLenum internal_format = gl::GLenum::GL_RGBA8);
    //! Creates array of 2D textures, represented as layers.
    Texture2D(const uint32_t width, const uint32_t height, const uint32_t layer_count, const gl::GLenum internal_format);

    virtual ~Texture2D() override;

    uint32_t GetWidth();
    uint32_t GetHeight();
    glm::uvec2 GetResolution();
    uint32_t GetLayerCount();

    void SetData(const gl::GLenum data_format, const gl::GLenum data_type, const void *data_ptr);
    void SetDataLayer(const uint32_t layer, const gl::GLenum data_format, const gl::GLenum data_type, const void *data_ptr);

    bool SaveToFile(const std::string &image_file_name, const bool confirm_on_overwrite = true);

  protected:

    gl::GLenum GetTarget() override;
    void ReserveMemory() override;
  private:
    gl::GLenum target_ = gl::GLenum::GL_TEXTURE_2D;
    uint32_t width_ = 0, height_ = 0;
    uint32_t layers_ = 1;
  };



  using PTexture2dMultisample = std::shared_ptr<class Texture2dMultisample>;



  //! Similar to Texture2D, uses multi-sampling for anti-aliasing effects.
  class GLW_API Texture2dMultisample : public Texture
  {
  public:
    Texture2dMultisample(const uint32_t width, const uint32_t height, const gl::GLenum internal_format, const uint8_t sample_count);
    uint8_t GetSampleCount();
    uint32_t GetWidth();
    uint32_t GetHeight();
  protected:
    gl::GLenum GetTarget() override;
    void ReserveMemory() override;
  private:
    //In shader sampler2DMS and texelFetch() required.
    gl::GLenum target_ = gl::GLenum::GL_TEXTURE_2D_MULTISAMPLE;
    uint32_t width_ = 0, height_ = 0;
    uint8_t sample_count_ = 0;
  };


}
