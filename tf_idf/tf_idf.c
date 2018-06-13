#define _CRT_SECURE_NO_WARNINGS
#define NUM 100

#include "mecab.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// df-idf �p
typedef struct idfStruct {
	char *word;     //�P��
	int df;     //�o�Ă�����
	float idfWeight;     // idf
	struct idfStruct *nextAddr;     //���̍\���̂̃A�h���X
}idfStruct;

// tf-idf �p
typedef struct tfStruct {
	char *word;     //�P��
	int tf;     //�o�Ă�����
	float tfWeight;     // tf
	float tfidfWeight;     // tf-idf
	struct tfStruct *nextAddr;     //���̍\���̂̃A�h���X
}tfStruct;

int main(int argc, char **argv)
{
	FILE *file;     //�t�@�C���|�C���^(�ǂ݂���)
	char idfFileName[512] = "..\\data\\idf.txt";    //�t�@�C�������i�[���邽�߂̔z��
	char tfFileName[512] = "";    //�t�@�C�������i�[���邽�߂̔z��

	idfStruct *idfwordsThis;    //���X�g�\���ō��w���Ă���\���̂̃A�h���X
	idfStruct *idfwordsList = NULL;
	idfStruct *idfwordsNew;

	tfStruct *tfwordsThis;    //���X�g�\���ō��w���Ă���\���̂̃A�h���X
	tfStruct *tfwordsList = NULL;
	tfStruct *tfwordsNew;

	char *newWord;

	int filenum;     //�t�@�C���ԍ�
	int listLength = 0;     //���X�g�̒���

	if ((file = fopen(idfFileName, "r")) == NULL)
	{
		printf("�ǂݍ��݃t�@�C�����J���܂���\n");
		exit(EXIT_FAILURE);
	}

	char word[256];
	int idfVol;
	float idfWeight;

	while (fscanf(file, "%s\t%d\t%f\n", word, &idfVol, &idfWeight) != EOF)
	{
		newWord = (char *)malloc(sizeof(char) * (strlen(word) + 1));
		strcpy(newWord, word);

		idfwordsThis = idfwordsList;
		while (1)
		{
			if (idfwordsThis == NULL)//�܂� 1 ���̓o�^���Ȃ����
			{
				break;
			}
			else if (strcmp(idfwordsThis->word, newWord) == 0)//���o������ƃ��X�g���̌ꂪ��v�����Ƃ�
			{
				break;
			}
			//���X�g�����ǂ邽�߂̃`�F�b�N
			if (idfwordsThis->nextAddr == NULL)//�������w���Ă���Ƃ�
			{
				break;
			}
			else//���̃A�h���X������̂ŁC���̃����N�ɐi��
			{
				idfwordsThis = idfwordsThis->nextAddr;
			}
		}

		//�V���Ȍ��ۑ����邽�߂̐V���ȍ\���̗̈���m�ۂ���imalloc�j
		idfwordsNew = (idfStruct *)malloc(sizeof(idfStruct) * 1);
		//�\���̂Ɍ��ۑ�����
		idfwordsNew->word = word;
		//�l�̃Z�b�g
		idfwordsNew->df = idfVol;
		idfwordsNew->idfWeight = idfWeight;
		//���̍\���̂������ɂȂ�̂ŁC���̎��̍\���̂̃A�h���X�� NULL �ɃZ�b�g����
		idfwordsNew->nextAddr = NULL;

		//���ꂪ���߂Ẵ��X�g�ł��鎞
		if (idfwordsList == NULL)
		{
			//���X�g�̐擪���w���|�C���^�ϐ��ɁC�V����������\���̂̃A�h���X��������
			idfwordsList = idfwordsNew;

		}
		else//���łɂ��郊�X�g�ɒǉ�����Ƃ�
		{
			//�����̍\���̂̃A�h���X�����ɁC�V����������\���̂̃A�h���X��������
			idfwordsThis->nextAddr = idfwordsNew;
		}
	}






	for (filenum = 1; filenum <= 100; filenum++)
	{
		sprintf(tfFileName, "%s%03d%s", "..\\data\\tf_", filenum, ".txt");
		if ((file = fopen(tfFileName, "r")) == NULL)
		{
			printf("�ǂݍ��݃t�@�C�����J���܂���\n");
			exit(EXIT_FAILURE);
		}

		int tfVol = 0;
		float tfWeight = 0;

		while (fscanf(file, "%s\t%d\t%f\n", word, &tfVol, &tfWeight) != EOF)
		{
			newWord = (char *)malloc(sizeof(char) * (strlen(word) + 1));
			strcpy(newWord, word);

			tfwordsThis = tfwordsList;
			while (1)
			{
				if (tfwordsThis == NULL)//�܂� 1 ���̓o�^���Ȃ����
				{
					break;
				}
				else if (strcmp(tfwordsThis->word, newWord) == 0)//���o������ƃ��X�g���̌ꂪ��v�����Ƃ�
				{
					break;
				}
				//���X�g�����ǂ邽�߂̃`�F�b�N
				if (tfwordsThis->nextAddr == NULL)//�������w���Ă���Ƃ�
				{
					break;
				}
				else//���̃A�h���X������̂ŁC���̃����N�ɐi��
				{
					tfwordsThis = tfwordsThis->nextAddr;
				}
			}

			//�V���Ȍ��ۑ����邽�߂̐V���ȍ\���̗̈���m�ۂ���imalloc�j
			tfwordsNew = (tfStruct *)malloc(sizeof(tfStruct) * 1);
			//�\���̂Ɍ��ۑ�����
			tfwordsNew->word = newWord;
			//�l�̃Z�b�g
			tfwordsNew->tf = tfVol;
			tfwordsNew->tfWeight = tfWeight;
			idfwordsThis = idfwordsList;
			while (1) {
				if (strcmp(tfwordsNew->word, idfwordsThis->word) != 0) {
						//�w�������A�h���X�����ɐi�߂�
						idfwordsThis = idfwordsThis->nextAddr;
				}
				else{
					tfwordsNew->tfidfWeight = tfwordsNew->tfWeight * idfwordsThis->idfWeight;
				}
			}
			//���̍\���̂������ɂȂ�̂ŁC���̎��̍\���̂̃A�h���X�� NULL �ɃZ�b�g����
			tfwordsNew->nextAddr = NULL;

			//���ꂪ���߂Ẵ��X�g�ł��鎞
			if (tfwordsList == NULL)
			{
				//���X�g�̐擪���w���|�C���^�ϐ��ɁC�V����������\���̂̃A�h���X��������
				tfwordsList = tfwordsNew;
			}
			else//���łɂ��郊�X�g�ɒǉ�����Ƃ�
			{
				//�����̍\���̂̃A�h���X�����ɁC�V����������\���̂̃A�h���X��������
				tfwordsThis->nextAddr = tfwordsNew;
			}
		}
	}













	return 0;
}