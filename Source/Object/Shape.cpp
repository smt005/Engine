
#include "Shape.h"
#include "FileManager.h"

struct BlockTemporary
{
	char* _name = nullptr;
	const int _maxCount = 10240;
	int _countLine = 0;
	char** _charLine = nullptr;

	int _countNumber = 9;

	int _count = 0;
	float* _floatArray = nullptr;
	unsigned short int* _intArray = nullptr;

	BlockTemporary()
	{
		_charLine = new char*[_maxCount];
	}

	BlockTemporary(int countNumber)
	{
		_countNumber = countNumber;
		_charLine = new char*[_maxCount];
	}

	~BlockTemporary()
	{
        delete[] _floatArray;
        delete[] _intArray;
		delete[] _charLine;
	}

	void parse()
	{
		_count = _countLine * _countNumber;

		if (_countNumber == 2 || _countNumber == 3)
		{
			_floatArray = new float[_count];

			for (int i = 0; i < _countLine; ++i)
			{
				int index = i * _countNumber;

				if (_countNumber == 2)
				{
					sscanf_s(_charLine[i], "%f %f", &_floatArray[index], &_floatArray[index + 1]);
				}
				else if (_countNumber == 3)
				{
					sscanf_s(_charLine[i], "%f %f %f", &_floatArray[index], &_floatArray[index + 1], &_floatArray[index + 2]);
				}
			}
		}
		else if (_countNumber == 9)
		{
			_intArray = new unsigned short int[_count];

			for (int i = 0; i < _countLine; ++i)
			{
				int intTemp[9];

				sscanf_s(_charLine[i], "%d/%d/%d %d/%d/%d %d/%d/%d",
					&intTemp[0],
					&intTemp[1],
					&intTemp[2],
					&intTemp[3],
					&intTemp[4],
					&intTemp[5],
					&intTemp[6],
					&intTemp[7],
					&intTemp[8]);

				int index = i * _countNumber;

				for (int i2 = 0; i2 < 9; ++i2)
				{
					_intArray[index + i2] = static_cast<unsigned short int>(intTemp[i2] - 1);
				}
			}
		}
	}

	static int foundIndex(float *vertex, float *normal, float *texCoord,
		float *aVertex, float *aNormal, float *aTexCoord,
		int count)
	{
		for (int index = 0; index < count; ++index)
		{
			if ((vertex[0] == aVertex[index * 3] && vertex[1] == aVertex[(index * 3) + 1] && vertex[2] == aVertex[(index * 3) + 2]) &&
				(normal[0] == aNormal[index * 3] && normal[1] == aNormal[(index * 3) + 1] && normal[2] == aNormal[(index * 3) + 2]) &&
				(texCoord[0] == aTexCoord[index * 2] && texCoord[1] == aTexCoord[(index * 2) + 1]))
			{
				return index;
			}
		}

		return -1;
	};

	static void getMesh(Mesh& mesh, int countIndexTemporary, BlockTemporary* indexTemporarys,
		BlockTemporary& vertexTemporary, BlockTemporary& normalTemporary, BlockTemporary& textureTemporary)
	{
		int countIndexT = 0;
		
		for (int iT = 0; iT < countIndexTemporary; ++iT)
		{
			countIndexT += indexTemporarys[iT]._count;
		}

		// TODO
		unsigned short* aIndexNew = new unsigned short[countIndexT * 3];
		float* aVertexNew = new float[countIndexT * 3];
		float* aNormalNew = new float[countIndexT * 3];
		float* aTextureNew = new float[countIndexT * 2];
		
		int indexVertexNew = 0;
		int iIndexNew = 0;

		float* aVertexT = vertexTemporary._floatArray;
		float* aTextureT = textureTemporary._floatArray;
		float* aNormalT = normalTemporary._floatArray;

		for (int iT = 0; iT < countIndexTemporary; ++iT)
		{
			BlockTemporary& indexTemporary = indexTemporarys[iT];
			unsigned short* aIndexT = indexTemporary._intArray;
			
			for (int i = 0; i < indexTemporary._count; i+=3)
			{
				unsigned short indexV = aIndexT[i];
				float* vertex = &aVertexT[indexV * 3];

				unsigned short indexT = aIndexT[i + 1];
				float* texCoord = &aTextureT[indexT * 2];

				unsigned short indexN = aIndexT[i + 2];
				float* normal = &aNormalT[indexN * 3];

				int index = foundIndex(vertex, normal, texCoord, aVertexNew, aNormalNew, aTextureNew, indexVertexNew);
				if (index == -1)
				{
					index = indexVertexNew;

					aVertexNew[index * 3] = vertex[0];
					aVertexNew[index * 3 + 1] = vertex[1];
					aVertexNew[index * 3 + 2] = vertex[2];

					aNormalNew[index * 3] = normal[0];
					aNormalNew[index * 3 + 1] = normal[1];
					aNormalNew[index * 3 + 2] = normal[2];

					aTextureNew[index * 2] = texCoord[0];
					aTextureNew[index * 2 + 1] = texCoord[1];

					++indexVertexNew;
				}

				aIndexNew[iIndexNew] = index;
				++iIndexNew;
			}
		}
        
		unsigned short* _aIndex = aIndexNew;

		float* aVertex = new float[indexVertexNew * 3];
		float* aNormal = new float[indexVertexNew * 3];
		float* aTexCoord = new float[indexVertexNew * 2];

		mesh.setData(aVertex, aNormal, aTexCoord, indexVertexNew, aIndexNew, iIndexNew);

		for (int i = 0; i < indexVertexNew; ++i)
		{
			int indexV = i * 3;

			aVertex[indexV] = aVertexNew[indexV];
			aVertex[indexV + 1] = aVertexNew[indexV + 1];
			aVertex[indexV + 2] = aVertexNew[indexV + 2];

			aNormal[indexV] = aNormalNew[indexV];
			aNormal[indexV + 1] = aNormalNew[indexV + 1];
			aNormal[indexV + 2] = aNormalNew[indexV + 2];

			int indexT = i * 2;

			aTexCoord[indexT] = aTextureNew[indexT];
			aTexCoord[indexT + 1] = aTextureNew[indexT + 1];
		}

		delete[] aVertexNew;
		delete[] aNormalNew;
		delete[] aTextureNew;
	}
};

Shape::~Shape()
{
}

bool Shape::create(const string &name)
{
	setName(name);
	return load(getName());
}

bool Shape::load(const string& name)
{
	char* data = nullptr;
	int lenght = 0;
	Engine::FileManager::readTextFile(name, data, lenght);

	if (!data) return false;

	int len = strlen(data);
	int iChar = 0;

	BlockTemporary vertexTemporary(3);
	BlockTemporary normalTemporary(3);
	BlockTemporary textureTemporary(2);

	const int maxCountIndexTemporary = 10;
	BlockTemporary* currentIndexTemporary = nullptr;

	int countIndexTemporary = 0;
	BlockTemporary indexTemporary[maxCountIndexTemporary];
	
	// -
	while (iChar < len)
	{
		if (data[iChar] == 'v' && data[iChar + 1] == ' ' && data[iChar + 2] == ' ')
		{
			vertexTemporary._charLine[vertexTemporary._countLine] = &data[iChar + 3];
			++vertexTemporary._countLine;
			iChar += 3;
		}

		if (data[iChar] == 'v' && data[iChar + 1] == 'n' && data[iChar + 2] == ' ')
		{
			normalTemporary._charLine[normalTemporary._countLine] = &data[iChar + 3];
			++normalTemporary._countLine;
			iChar += 3;
		}

		if (data[iChar] == 'v' && data[iChar + 1] == 't' && data[iChar + 2] == ' ')
		{
			textureTemporary._charLine[textureTemporary._countLine] = &data[iChar + 3];
			++textureTemporary._countLine;
			iChar += 3;
		}

		if (data[iChar] == 'f' && data[iChar + 1] == ' ')
		{
			if (!currentIndexTemporary)
			{
				char* name = nullptr;
				int iCharTemp = iChar - 1;
				data[iCharTemp] = '\0';

				while (iCharTemp >= 0 && data[iCharTemp] != ' ') {
					--iCharTemp;
				}

				name = &data[iCharTemp + 1];

				currentIndexTemporary = &indexTemporary[countIndexTemporary];
				++countIndexTemporary;

				if (countIndexTemporary >= maxCountIndexTemporary) {
					break;
				}

				currentIndexTemporary->_name = name;
			}

			currentIndexTemporary->_charLine[currentIndexTemporary->_countLine] = &data[iChar + 2];
			++currentIndexTemporary->_countLine;
			iChar += 2;
		}

		if (currentIndexTemporary && (data[iChar] == '#' && data[iChar + 1] == ' '))
		{
			currentIndexTemporary = nullptr;
			iChar += 2;
		}

		if (data[iChar] == '\n')
		{
			data[iChar] = '\0';
		}

		++iChar;
	}
    
	vertexTemporary.parse();
	normalTemporary.parse();
	textureTemporary.parse();

	for (int i = 0; i < countIndexTemporary; ++i)
	{
		indexTemporary[i].parse();
	}

	BlockTemporary::getMesh(*this, countIndexTemporary, indexTemporary, vertexTemporary, normalTemporary, textureTemporary);

	return true;
}

void Shape::setScale(float* scale)
{
	for (int i = 0; i < _countVertex * 3; i+=3)
	{
		_aVertex[i    ] = _aVertex[i    ] * scale[0];
		_aVertex[i + 1] = _aVertex[i + 1] * scale[1];
		_aVertex[i + 2] = _aVertex[i + 2] * scale[2];
	}
}
