#include "objmodel.h"
#include "utils.h"
#include <string>
#include <sstream>
#include <vector>

void ObjModel::Init(const char*modelPath)
{
	struct FloatData
	{
		float v[3];
	};
	struct VerticeDefine
	{
		int posIndex;
		int texcoordIndex;
		int normalIndex;
	};
	std::vector<FloatData> positions, texcoords, normals;
	std::vector<VerticeDefine> vertexes;//unique vertex
	std::vector<int> indexes;//


	int nFileSize = 0;
	unsigned char*fileContent = LoadFileContent(modelPath, nFileSize);
	if (fileContent == nullptr) {
		return;
	}

	std::stringstream ssFileContent((char*)fileContent);
	std::string temp;
	char szOneLine[256];// 一行256字节读取

	while (!ssFileContent.eof())
	{
		memset(szOneLine, 0, 256);
		ssFileContent.getline(szOneLine, 256);
		if (strlen(szOneLine) > 0)
		{
			if (szOneLine[0] == 'v')
			{
				std::stringstream ssOneLine(szOneLine);

				if (szOneLine[1] == 't') //'vt' 顶点的纹理坐标
				{
					ssOneLine >> temp;
					FloatData floatData;
					ssOneLine >> floatData.v[0];
					ssOneLine >> floatData.v[1];
					//printf("texcoord : %f,%f\n", floatData.v[0], floatData.v[1]);
					texcoords.push_back(floatData);
				}
				else if (szOneLine[1] == 'n')//'vn' 顶点的法线
				{
					ssOneLine >> temp;
					FloatData floatData;
					ssOneLine >> floatData.v[0];
					ssOneLine >> floatData.v[1];
					ssOneLine >> floatData.v[2];
					normals.push_back(floatData);
					//printf("normal : %f,%f,%f\n", floatData.v[0], floatData.v[1], floatData.v[2]);
				}
				else  //'v' 顶点坐标
				{
					ssOneLine >> temp;
					FloatData floatData;
					ssOneLine >> floatData.v[0];
					ssOneLine >> floatData.v[1];
					ssOneLine >> floatData.v[2];
					positions.push_back(floatData);
					//printf("position : %f,%f,%f\n", floatData.v[0], floatData.v[1], floatData.v[2]);
				}
			}
			else if (szOneLine[0] == 'f') //'f' 组成面里的点的信息 f 1/1/1
			{
				std::stringstream ssOneLine(szOneLine);
				ssOneLine >> temp;//
				std::string vertexStr;// 1/1/1
				for (int i = 0; i < 3; i++)
				{
					ssOneLine >> vertexStr;

					//find verticeDefine index info
					size_t pos = vertexStr.find_first_of('/');
					std::string posIndexStr = vertexStr.substr(0, pos);

					size_t pos2 = vertexStr.find_first_of('/', pos + 1);
					std::string texcoordIndexStr = vertexStr.substr(pos + 1, pos2 - 1 - pos);
					std::string normalIndexStr = vertexStr.substr(pos2 + 1, vertexStr.length() - 1 - pos2);

					VerticeDefine vd;
					vd.posIndex = atoi(posIndexStr.c_str());
					vd.texcoordIndex = atoi(texcoordIndexStr.c_str());
					vd.normalIndex = atoi(normalIndexStr.c_str());
					//printf("%d,%d,%d\n", vd.posIndex, vd.texcoordIndex, vd.normalIndex);

					//去重
					int nCurrentVertexIndex = -1;
					int nCurrentVertexCount = (int)vertexes.size();
					for (int j = 0; j < nCurrentVertexCount; ++j)
					{
						if (vertexes[j].posIndex == vd.posIndex&&
							vertexes[j].normalIndex == vd.normalIndex&&
							vertexes[j].texcoordIndex == vd.texcoordIndex)
						{
							nCurrentVertexIndex = j;
							break;
						}
					}
					if (nCurrentVertexIndex == -1)
					{
						nCurrentVertexIndex = (int)vertexes.size();
						vertexes.push_back(vd);
					}
					indexes.push_back(nCurrentVertexIndex);
				}
				//printf("face : %s\n", szOneLine);
			}
		}
	}
	printf("unique vertex count %u index count %u\n", vertexes.size(), indexes.size());
	mIndexCount = indexes.size();
	mIndexs = new int[mIndexCount];
	for (int i = 0; i < mIndexCount; i++)
	{
		mIndexs[i] = indexes[i];
	}

	int vertexcount = (int)vertexes.size();
	mVertexes = new VertexData[vertexcount];
	for (int i = 0; i < vertexcount; ++i)
	{
		memcpy(mVertexes[i].position, positions[vertexes[i].posIndex - 1].v, sizeof(float) * 3);
		memcpy(mVertexes[i].texcoord, texcoords[vertexes[i].texcoordIndex - 1].v, sizeof(float) * 2);
		memcpy(mVertexes[i].normal, normals[vertexes[i].normalIndex - 1].v, sizeof(float) * 3);
	}

	delete fileContent;

	mObjModel.Init([&]()->void {
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < mIndexCount; ++i)
		{
			glTexCoord2fv(mVertexes[mIndexs[i]].texcoord);
			glNormal3fv(mVertexes[mIndexs[i]].normal);
			glVertex3fv(mVertexes[mIndexs[i]].position);
		}
		glEnd();
	});
}

void ObjModel::Draw()
{
	//static float angle = 0.0f;
	//angle += 0.2f;
	//if (angle >= 360.0f)
	//{
	//	angle = 0.0f;
	//}

	//glPolygonMode(GL_FRONT, GL_LINE);
	glPushMatrix();
	mObjModel.Draw();
	glPopMatrix();
}