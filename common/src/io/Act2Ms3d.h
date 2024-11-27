/**
 * Act2MilkShape
 * Simple converter from Genesis3D .act to MilkShape .ms3d
 *
 * Copyright (c) 2024 rtxa
 * This code is licensed under the MIT license (see LICENSE.txt for details).
 */

#pragma once

#include <GenesisSDK/XFORM3D.H>
#include <GenesisSDK/basetype.h>
#include <GenesisSDK/body.h>
#include <GenesisSDK/genesis.h>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#pragma pack(1)
struct ms3d_header_t
{
  char id[10]; // always "MS3D000000"
  int version; // 4
};

struct ms3d_vertex_t
{
  uint8_t flags;          // Selected | HIDDEN
  float vertex[3];        // Position
  char boneId;            // Bone index or -1
  uint8_t referenceCount; // Reference count
};

struct ms3d_triangle_t
{
  uint16_t flags;            // SELECTED | SELECTED2 | HIDDEN
  uint16_t vertexIndices[3]; // Indices of vertices
  float vertexNormals[3][3]; // Normals for each vertex
  float s[3];                // Texture coordinates (u)
  float t[3];                // Texture coordinates (v)
  uint8_t smoothingGroup;    // Smoothing group: 1 - 32
  uint8_t groupIndex;        // Material index
};

struct ms3d_material_t
{
  char name[32];      // Material name
  float ambient[4];   // Ambient color
  float diffuse[4];   // Diffuse color
  float specular[4];  // Specular color
  float emissive[4];  // Emissive color
  float shininess;    // Shininess factor 0.0f - 128.0f
  float transparency; // Transparency 0.0f - 1.0f
  char mode;          // Material mode - 0, 1, 2 is unused now
  char texture[128];  // Texture file name
  char alphamap[128]; // Alpha map file name
};

struct ms3d_joint_t
{
  uint8_t flags;              // SELECTED | DIRTY
  char name[32];              // Joint name
  char parentName[32];        // Parent joint name
  float rotation[3];          // Local rotation
  float position[3];          // Local position
  uint16_t numKeyFramesRot;   // Number of rotation keyframes
  uint16_t numKeyFramesTrans; // Number of translation keyframes

  // Local animation matrices
  // ms3d_keyframe_rot keyFramesRot[numKeyFramesRot];

  // Local animation matrices
  // ms3d_keyframe_pos keyFramesTrans[numKeyFramesTrans];
};

struct ms3d_group_t
{
  uint8_t flags{0};                      // SELECTED | HIDDEN
  char name[32];                         //
  uint16_t numtriangles{0};              //
  std::vector<uint16_t> triangleIndices; // Dynamic
  char materialIndex{0};                 // -1 = no material
};

struct ms3d_anim_controls_t
{
  float animationFPS;
  float currentTime;
  uint32_t totalFrames;
};

struct ms3d_model_t
{
  ms3d_header_t header;
  std::vector<ms3d_vertex_t> vertices;
  std::vector<ms3d_triangle_t> triangles;
  std::vector<ms3d_group_t> groups;
  std::vector<ms3d_material_t> materials;
  ms3d_anim_controls_t animControls;
  std::vector<ms3d_joint_t> joints;

  ms3d_model_t(
    size_t vertexCount, size_t triangleCount, size_t materialCount, size_t jointCount)
    : vertices(vertexCount)
    , triangles(triangleCount)
    , materials(materialCount)
    , joints(jointCount)
  {
  }
};

#pragma pack()

typedef int16_t geBody_Index;

struct geStrBlock
{
  int Count;
  geStrBlock* SanityCheck;
  union
  {
    int IntArray[1]; // char offset into CharArray for string[n]
    char CharArray[1];
  } Data;
};

struct geBody_XSkinVertex
{
  geVec3d XPoint;
  geFloat XU, XV;
  int8 LevelOfDetailMask;
  geBody_Index BoneIndex;
};

struct geBody_Normal
{
  geVec3d Normal;
  int8 LevelOfDetailMask;
  geBody_Index BoneIndex;
};

struct geBody_Bone
{
  geVec3d BoundingBoxMin;
  geVec3d BoundingBoxMax;
  geXForm3d AttachmentMatrix;
  geBody_Index ParentBoneIndex;
};

struct geBody_Triangle
{
  geBody_Index VtxIndex[3];
  geBody_Index NormalIndex[3];
  geBody_Index MaterialIndex;
  // geBody_Index	FaceNormal;
};

struct geBody_TriangleList
{
  geBody_Index FaceCount;
  geBody_Triangle* FaceArray; // Sorted by MaterialIndex
};

struct geBody_Material
{
  geBitmap* Bitmap;
  geFloat Red, Green, Blue;
};

struct geBody
{
  geVec3d BoundingBoxMin;
  geVec3d BoundingBoxMax;

  geBody_Index XSkinVertexCount;
  geBody_XSkinVertex* XSkinVertexArray; // Sorted by BoneIndex

  geBody_Index SkinNormalCount;
  geBody_Normal* SkinNormalArray;

  geBody_Index BoneCount;
  geBody_Bone* BoneArray;
  geStrBlock* BoneNames;

  geBody_Index MaterialCount;
  geBody_Material* MaterialArray;
  geStrBlock* MaterialNames;

  int LevelsOfDetail;
  geBody_TriangleList SkinFaces[GE_BODY_NUMBER_OF_LOD];

  geBody* IsValid;
};

const char* GENESISCC geStrBlock_GetString(const geStrBlock* SB, int Index)
{
  assert(SB != NULL);
  assert(Index >= 0);
  assert(Index < SB->Count);
  assert(SB->SanityCheck == SB);
  return &(SB->Data.CharArray[SB->Data.IntArray[Index]]);
}

namespace act2ms3d
{
template <typename T>
void WriteToBuffer(std::vector<char>& buffer, const T& data)
{
  size_t offset = buffer.size();
  buffer.resize(offset + sizeof(T));
  std::memcpy(buffer.data() + offset, reinterpret_cast<const char*>(&data), sizeof(T));
}

template <typename T, typename SizeType>
void WriteVectorToBuffer(std::vector<char>& buffer, const std::vector<T>& data)
{
  SizeType count = static_cast<SizeType>(data.size());
  WriteToBuffer(buffer, count); // Write the count first
  size_t offset = buffer.size();
  buffer.resize(offset + sizeof(T) * count);
  std::memcpy(buffer.data() + offset, data.data(), sizeof(T) * count);
}

void WriteMS3DToBuffer(std::vector<char>& buffer, const ms3d_model_t& model)
{
  // 1. Write the header
  WriteToBuffer(buffer, model.header);

  // 2. Write vertices
  WriteVectorToBuffer<ms3d_vertex_t, uint16_t>(buffer, model.vertices);

  // 3. Write triangles
  WriteVectorToBuffer<ms3d_triangle_t, uint16_t>(buffer, model.triangles);

  // 4. Write groups
  auto groupCount = static_cast<uint16_t>(model.groups.size());
  WriteToBuffer(buffer, groupCount);
  for (const auto& group : model.groups)
  {
    WriteToBuffer(buffer, group.flags);
    buffer.insert(buffer.end(), group.name, group.name + 32); // Copy group name
    WriteToBuffer(buffer, group.numtriangles);
    for (uint16_t index : group.triangleIndices)
    {
      WriteToBuffer(buffer, index);
    }
    WriteToBuffer(buffer, group.materialIndex);
  }

  // 5. Write materials
  WriteVectorToBuffer<ms3d_material_t, uint16_t>(buffer, model.materials);

  // 6. Write editor animation controls
  WriteToBuffer(buffer, model.animControls);

  // 7. Write joints
  WriteVectorToBuffer<ms3d_joint_t, uint16_t>(buffer, model.joints);
}

std::vector<std::string> GetMaterialNames(geStrBlock* materialNames)
{
  if (!materialNames)
    return {}; // Check for null pointer
  std::vector<std::string> names;
  for (int i = 0; i < materialNames->Count; ++i)
  {
    int offset = materialNames->Data.IntArray[i];
    const char* name = &materialNames->Data.CharArray[offset];
    names.push_back(name);
  }
  return names;
}

// Function to recursively get the global transformation matrix for a bone
void GetGlobalTransformationMatrix(
  int boneIndex, geXForm3d* globalMatrix, const geBody* body)
{
  if (boneIndex == -1)
    return; // No bone index

  geXForm3d localMatrix = body->BoneArray[boneIndex].AttachmentMatrix;

  // If the bone has a parent, get its global transformation
  int parentBoneIndex =
    body->BoneArray[boneIndex]
      .ParentBoneIndex; // Assuming you have a way to get the parent index
  if (parentBoneIndex != -1)
  {
    geXForm3d parentGlobal;
    GetGlobalTransformationMatrix(parentBoneIndex, &parentGlobal, body);
    geXForm3d_Multiply(&parentGlobal, &localMatrix, globalMatrix);
  }
  else
  {
    // If no parent, the global matrix is just the local one
    *globalMatrix = localMatrix;
  }
}

void QuaternionToEuler(const geQuaternion* q, geVec3d* angles)
{
  constexpr auto M_PI = 3.14159265358979323846;

  // Roll (X)
  double sinr_cosp = 2.0 * (q->W * q->X + q->Y * q->Z);
  double cosr_cosp = 1.0 - 2.0 * (q->X * q->X + q->Y * q->Y);
  double roll = atan2(sinr_cosp, cosr_cosp);

  // Pitch (Y)
  double sinp = 2.0 * (q->W * q->Y - q->Z * q->X);
  double pitch;
  if (fabs(sinp) >= 1.0)
  {
    pitch = copysign(M_PI / 2, sinp); // Use 90 degrees if out of range
  }
  else
  {
    pitch = asin(sinp);
  }

  // Yaw (Z)
  double siny_cosp = 2.0 * (q->W * q->Z + q->X * q->Y);
  double cosy_cosp = 1.0 - 2.0 * (q->Y * q->Y + q->Z * q->Z);
  double yaw = atan2(siny_cosp, cosy_cosp);

  // Convert to float and assign
  angles->X = static_cast<float>(roll);
  angles->Y = static_cast<float>(pitch);
  angles->Z = static_cast<float>(yaw);
}

void ConvertGeBodyToMS3D(const geBody* body, ms3d_model_t& model)
{
  // 1. Fill header
  strcpy(model.header.id, "MS3D000000");
  model.header.version = 4; // MS3D version

  // 2. Fill vertices
  auto vertexCount = static_cast<size_t>(body->XSkinVertexCount);
  for (size_t i = 0; i < vertexCount; ++i)
  {
    model.vertices[i].flags = 0; // Default flags

    // Get the bone index for this vertex
    int boneIndex = body->XSkinVertexArray[i].BoneIndex;

    // Retrieve the global transformation matrix for the corresponding bone
    geXForm3d globalMatrix;
    GetGlobalTransformationMatrix(boneIndex, &globalMatrix, body);

    // Create the original vertex vector
    geVec3d originalVertex = body->XSkinVertexArray[i].XPoint;

    // Transform the vertex position using the global transformation matrix
    geVec3d transformedVertex;
    geXForm3d_Transform(&globalMatrix, &originalVertex, &transformedVertex);

    // Set the transformed vertex position
    model.vertices[i].vertex[0] = transformedVertex.X;
    model.vertices[i].vertex[1] = transformedVertex.Y;
    model.vertices[i].vertex[2] = transformedVertex.Z;

    model.vertices[i].boneId = boneIndex;
    model.vertices[i].referenceCount = 0;
  }

  // 3. Fill triangles
  auto triangleCount = static_cast<size_t>(body->SkinFaces[0].FaceCount);
  for (size_t i = 0; i < triangleCount; ++i)
  {
    model.triangles[i].flags = 0;
    const auto& triangle = body->SkinFaces[0].FaceArray[i];

    model.triangles[i].vertexIndices[0] = static_cast<uint16_t>(triangle.VtxIndex[0]);
    model.triangles[i].vertexIndices[1] = static_cast<uint16_t>(triangle.VtxIndex[1]);
    model.triangles[i].vertexIndices[2] = static_cast<uint16_t>(triangle.VtxIndex[2]);

    for (int j = 0; j < 3; ++j)
    {
      int normalIndex = triangle.NormalIndex[j];
      model.triangles[i].vertexNormals[j][0] =
        body->SkinNormalArray[normalIndex].Normal.X;
      model.triangles[i].vertexNormals[j][1] =
        body->SkinNormalArray[normalIndex].Normal.Y;
      model.triangles[i].vertexNormals[j][2] =
        body->SkinNormalArray[normalIndex].Normal.Z;

      int vtxIndex = triangle.VtxIndex[j];
      model.triangles[i].s[j] = body->XSkinVertexArray[vtxIndex].XU;
      model.triangles[i].t[j] = body->XSkinVertexArray[vtxIndex].XV;
    }

    model.triangles[i].smoothingGroup = 1; // Default smoothing group
    model.triangles[i].groupIndex = 0;     // Default group index
  }

  // 4. Fill groups based on MaterialIndex
  std::vector<ms3d_group_t> groupList;
  ms3d_group_t currentGroup{};
  int numGlobalTriangles = 0;

  // Initialize the first group
  currentGroup.materialIndex =
    static_cast<char>(body->SkinFaces[0].FaceArray[0].MaterialIndex);
  sprintf(currentGroup.name, "Group %d", groupList.size() + 1);

  for (int i = 0; i < body->SkinFaces[0].FaceCount; ++i)
  {
    const auto& triangle = body->SkinFaces[0].FaceArray[i];

    // Check if we need to start a new group
    if (triangle.MaterialIndex != currentGroup.materialIndex)
    {
      groupList.push_back(currentGroup);
      currentGroup = {}; // Clear for the next group
      currentGroup.materialIndex = static_cast<char>(triangle.MaterialIndex);
      sprintf(currentGroup.name, "Group %d", groupList.size() + 1);
    }

    currentGroup.triangleIndices.push_back(static_cast<uint16_t>(numGlobalTriangles++));
    currentGroup.numtriangles++;
  }

  // Push the last group if it contains triangles
  if (currentGroup.numtriangles > 0)
  {
    groupList.push_back(currentGroup);
  }
  model.groups = groupList;

  // 5. Fill materials
  std::vector<std::string> names = GetMaterialNames(body->MaterialNames);
  auto materialCount = static_cast<size_t>(body->MaterialCount);
  for (size_t i = 0; i < materialCount; ++i)
  {
    auto& mat = model.materials[i];
    mat.ambient[0] = mat.ambient[1] = mat.ambient[2] = mat.ambient[3] = 1.0f;
    mat.diffuse[0] = mat.diffuse[1] = mat.diffuse[2] = mat.diffuse[3] = 1.0f;
    mat.specular[0] = mat.specular[1] = mat.specular[2] = 0.0f;
    mat.specular[3] = 1.0f;
    mat.emissive[0] = mat.emissive[1] = mat.emissive[2] = 0.0f;
    mat.emissive[3] = 1.0f;
    mat.shininess = 0.0f;
    mat.transparency = 1.0f;
    mat.mode = 2;

    sprintf(mat.name, "%s", names[i].c_str());
    sprintf(mat.texture, "%s.bmp", names[i].c_str());
    mat.alphamap[0] = '\x00';
  }

  // 6. Set editor animation controls
  model.animControls.animationFPS = 0.0f;
  model.animControls.currentTime = 0.0f;
  model.animControls.totalFrames = 0;

  // 7. Fill joints
  auto jointsCount = static_cast<size_t>(body->BoneCount);
  for (size_t i = 0; i < jointsCount; ++i)
  {
    model.joints[i].flags = 0;
    geBody_Bone* bone = &body->BoneArray[i];

    // Set joint names
    strcpy(
      model.joints[i].name, geStrBlock_GetString(body->BoneNames, static_cast<int>(i)));
    if (bone->ParentBoneIndex == -1)
    {
      model.joints[i].parentName[0] = '\x00'; // No parent
    }
    else
    {
      strcpy(
        model.joints[i].parentName,
        geStrBlock_GetString(body->BoneNames, bone->ParentBoneIndex));
    }

    // Set translation from the attachment matrix
    model.joints[i].position[0] = bone->AttachmentMatrix.Translation.X;
    model.joints[i].position[1] = bone->AttachmentMatrix.Translation.Y;
    model.joints[i].position[2] = bone->AttachmentMatrix.Translation.Z;

    // Extract rotation from the attachment matrix
    geQuaternion quaternion;
    geQuaternion_FromMatrix(&bone->AttachmentMatrix, &quaternion);

    // Convert quaternion to Euler angles
    geVec3d angles;
    QuaternionToEuler(&quaternion, &angles);

    // Store Euler angles in the joint structure
    model.joints[i].rotation[0] = angles.X;
    model.joints[i].rotation[1] = angles.Y;
    model.joints[i].rotation[2] = angles.Z;

    // No animations for now
    model.joints[i].numKeyFramesRot = 0;
    model.joints[i].numKeyFramesTrans = 0;
  }
}
} // namespace act2ms3d