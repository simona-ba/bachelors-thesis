/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <GLW/ShaderProgram.h>

namespace glw
{

  #pragma region Shader Pipeline structure

  struct GLW_API ShaderPipeline
  {
    ShaderPipeline() = default;

    ShaderPipeline(const std::string &base_filename);

    std::map<glw::ShaderType, std::string> GetAll() const { return data_; };

    std::string Get(glw::ShaderType type) { return data_[type]; };

    __declspec(property(get = GetVert, put = SetVert)) std::string vert;
    __declspec(property(get = GetTesc, put = SetTesc)) std::string tesc;
    __declspec(property(get = GetTese, put = SetTese)) std::string tese;
    __declspec(property(get = GetGeom, put = SetGeom)) std::string geom;
    __declspec(property(get = GetFrag, put = SetFrag)) std::string frag;

    #pragma endregion Property setters and getters
    std::string GetVert() const { return data_.at(glw::ShaderType::VERTEX); };
    std::string GetTesc() const { return data_.at(glw::ShaderType::TESS_CTRL); };
    std::string GetTese() const { return data_.at(glw::ShaderType::TESS_EVAL); };
    std::string GetGeom() const { return data_.at(glw::ShaderType::GEOMETRY); };
    std::string GetFrag() const { return data_.at(glw::ShaderType::FRAGMENT); };
    void SetVert(const std::string &value) { data_[glw::ShaderType::VERTEX] = value; };
    void SetTesc(const std::string &value) { data_[glw::ShaderType::TESS_CTRL] = value; };
    void SetTese(const std::string &value) { data_[glw::ShaderType::TESS_EVAL] = value; };
    void SetGeom(const std::string &value) { data_[glw::ShaderType::GEOMETRY] = value; };
    void SetFrag(const std::string &value) { data_[glw::ShaderType::FRAGMENT] = value; };
    #pragma endregion
  private:
    std::map<glw::ShaderType, std::string> data_
    {
      { glw::ShaderType::VERTEX, "" },
      { glw::ShaderType::TESS_CTRL, "" },
      { glw::ShaderType::TESS_EVAL, "" },
      { glw::ShaderType::GEOMETRY, "" },
      { glw::ShaderType::FRAGMENT, "" }
    };
  };
  #pragma endregion

  using PProgramList = std::shared_ptr<class ProgramList>;

  class GLW_API ProgramList
  {
  public:

    static inline PProgramList Create()
    {
      return std::make_shared<ProgramList>();
    };

  public:

    ProgramList() = default;

    ~ProgramList();

    void ClearAll();

    void Reload();

    void AddFromFile(std::string program_name, const ShaderPipeline &files, const std::map<std::string, std::string> replace_map = std::map<std::string, std::string>());

    void AddFromSource(std::string program_name, const ShaderPipeline &sources, const std::map<std::string, std::string> replace_map = std::map<std::string, std::string>());

    bool Exist(const std::string &program_name);

    void Use(const std::string &program_name) const;

    std::vector<std::string> GetAllProgramNames() const;

    glw::PProgram GetProgram(const std::string &program_name);

    bool IsEmpty();

  private:
    std::map<std::string, glw::PProgram> programs_;
    std::map<std::string, ShaderPipeline> shader_files_;

    bool LinkAndShowMessages(const glw::PProgram &program_ptr, const std::string &program_name);
    glw::PProgram &CreateProgram(const std::string &id);
  };

  extern ProgramList global_program;

}