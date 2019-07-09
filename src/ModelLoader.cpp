#include "ModelLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/version.h>
#include <assimp/cimport.h>
#include <iostream>
#include "GeometryData.h"

using namespace std;

std::vector<GeometryPtr> ModelLoader::loadGeometries(const std::string &path) {
    struct aiLogStream stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
    aiAttachLogStream(&stream);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_CalcTangentSpace
    );

    std::vector<GeometryPtr> geometries;

    if (scene && scene->mFlags != AI_SCENE_FLAGS_INCOMPLETE) {
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            std::cout << "Loading geometry " << i << "/" << scene->mNumMeshes << endl;
            geometries.push_back(loadGeometry(scene->mMeshes[i]));
        }
    } else {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
    }

    aiDetachAllLogStreams();

    return geometries;
}

GeometryPtr ModelLoader::loadGeometry(const aiMesh *mesh) {
    GeometryData geometryData(mesh->mNumVertices, mesh->mNumFaces * 3);

    cout << "faces=" << mesh->HasFaces() << " normals=" << mesh->HasNormals() << " positions=" << mesh->HasPositions() << " tangents=" << mesh->HasTangentsAndBitangents() << " uvs=" << mesh->HasTextureCoords(0) << endl;

    for(GLuint i = 0; i < mesh->mNumVertices; i++) {
        geometryData.position(i, mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        if (mesh->HasNormals()) {
            geometryData.normal(i, mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }

        if (mesh->HasTangentsAndBitangents()) {
            geometryData.tangent(i, mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            geometryData.bitangent(i, mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        }

        if (mesh->HasTextureCoords(0)) {
            geometryData.uv(i, mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
    }

    for(GLuint i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        geometryData.triangle(i, face.mIndices[0], face.mIndices[1], face.mIndices[2]);
    }

    return geometryData.build();
}
