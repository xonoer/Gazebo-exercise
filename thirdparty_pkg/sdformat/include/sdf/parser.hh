/*
 * Copyright 2012 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#ifndef _SDF_PARSER_HH_
#define _SDF_PARSER_HH_

#include <tinyxml.h>
#include <string>

#include "sdf/SDFImpl.hh"
#include "sdf/system_util.hh"

/// \ingroup sdf_parser
/// \brief namespace for Simulation Description Format parser
namespace sdf
{
  /// \brief Init based on the installed sdf_format.xml file
  SDFORMAT_VISIBLE
  bool init(SDFPtr _sdf);

  // \brief Initialize the SDF interface using a file
  SDFORMAT_VISIBLE
  bool initFile(const std::string &_filename, SDFPtr _sdf);

  // \brief Initialize and SDFElement interface using a file
  SDFORMAT_VISIBLE
  bool initFile(const std::string &_filename, ElementPtr _sdf);

  // \brief Initialize the SDF interface using a string
  SDFORMAT_VISIBLE
  bool initString(const std::string &_xmlString, SDFPtr _sdf);

  // \brief Initialize the SDF interface using a TinyXML document
  SDFORMAT_VISIBLE
  bool initDoc(TiXmlDocument *_xmlDoc, SDFPtr _sdf);

  // \brief Initialize and SDF Element using a TinyXML document
  SDFORMAT_VISIBLE
  bool initDoc(TiXmlDocument *_xmlDoc, ElementPtr _sdf);

  // \brief For internal use only. Do not use this function.
  SDFORMAT_VISIBLE
  bool initXml(TiXmlElement *_xml, ElementPtr _sdf);

  /// \brief Populate the SDF values from a file
  SDFORMAT_VISIBLE
  bool readFile(const std::string &_filename, SDFPtr _sdf);

  /// \brief Populate the SDF values from a string
  SDFORMAT_VISIBLE
  bool readString(const std::string &_xmlString, SDFPtr _sdf);

  SDFORMAT_VISIBLE
  bool readString(const std::string &_xmlString, ElementPtr _sdf);

  /// \brief Populate the SDF values from a TinyXML document
  SDFORMAT_VISIBLE
  bool readDoc(TiXmlDocument *_xmlDoc, SDFPtr _sdf, const std::string &_source);

  SDFORMAT_VISIBLE
  bool readDoc(TiXmlDocument *_xmlDoc, ElementPtr _sdf,
               const std::string &_source);

  // \brief For internal use only. Do not use this function.
  SDFORMAT_VISIBLE
  bool readXml(TiXmlElement *_xml, ElementPtr _sdf);

  /// \brief Get the best SDF version from models supported by this sdformat
  /// \param[in] _modelXML XML element from config file pointing to the
  ///            model XML tag
  /// \param[out] _modelFileName file name of the best model file
  /// \return string with the best SDF version supported
  SDFORMAT_VISIBLE
  std::string getBestSupportedModelVersion(TiXmlElement *_modelXML,
                                           std::string &_modelFileName);

  /// \brief Get the file path to the model file
  /// \param[in] _modelDirPath directory system path of the model
  /// \return string with the full filesystem path to the best version (greater
  ///         SDF protocol supported by this sdformat version) of the .sdf
  ///         model files hosted by _modelDirPath.
  SDFORMAT_VISIBLE
  std::string getModelFilePath(const std::string &_modelDirPath);

  SDFORMAT_VISIBLE
  void copyChildren(ElementPtr _sdf, TiXmlElement *_xml);

  SDFORMAT_VISIBLE
  void addNestedModel(ElementPtr _sdf, ElementPtr _includeSDF);

  /// \brief Convert an SDF file to a specific SDF version.
  /// \param[in] _filename Name of the SDF file to convert.
  /// \param[in] _version Version to convert _filename to.
  /// \param[out] _sdf Pointer to the converted SDF document.
  /// \return True on success.
  SDFORMAT_VISIBLE
  bool convertFile(const std::string &_filename, const std::string &_version,
                   SDFPtr _sdf);

  /// \brief Convert an SDF string to a specific SDF version.
  /// \param[in] _sdfString The SDF string to convert.
  /// \param[in] _version Version to convert _filename to.
  /// \param[out] _sdf Pointer to the converted SDF document.
  /// \return True on success.
  SDFORMAT_VISIBLE
  bool convertString(const std::string &_sdfString,
                     const std::string &_version, SDFPtr _sdf);
}
#endif
