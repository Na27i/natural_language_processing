#define _CRT_SECURE_NO_WARNINGS
#define NUM 100

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct idfStruct {
	char *word;     //�P��
	int df;     //df
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
	FILE *file;     //�t�@�C���|�C���^
	FILE *outputfile;     //�t�@�C���|�C���^
	char fileName[512] = "";    //�t�@�C�������i�[���邽�߂̔z��
	char outputFileName[512] = "";    //�t�@�C�������i�[���邽�߂̔z��

	idfStruct *idfwordsThis;    //���X�g�\���ō��w���Ă���\���̂̃A�h���X
	idfStruct *idfwordsList = NULL;
	idfStruct *idfwordsNew;

	tfStruct *tfwordsThis;    //���X�g�\���ō��w���Ă���\���̂̃A�h���X
	tfStruct *tfwordsList;
	tfStruct *tfwordsNew;

	tfStruct *wordsTemp;     //����ւ��Ȃǂ̎��Ɉꎞ�I�ɍ\���̂̃A�h���X������
	tfStruct *wordsPre;     //wordsThis �̃A�h���X�������Ă���\���̂̃A�h���X������(�����p�̉��u����)
	tfStruct *wordsNext;    //���w���Ă���\���̂̎��̃A�h���X������

	char *newWord;

	int filenum;     //�t�@�C���ԍ�

	int listLength = 0;     //���X�g�̒���


	//�@���@idf�̃��X�g�쐬�@��
	sprintf(fileName, "%s", "..\\data\\df_new.txt");
	if ((file = fopen(fileName, "r")) == NULL)
	{
		printf("�ǂݍ��݃t�@�C�����J���܂���\n");
		exit(EXIT_FAILURE);
	}

	char word[256];
	int dfVol = 0;
	float idfWeight = 0;

	while (fscanf(file, "%s\t%d\t%f\n", word, &dfVol, &idfWeight) != EOF)
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
		idfwordsNew = (idfStruct *)malloc(sizeof(idfStruct));
		//�\���̂Ɍ��ۑ�����
		idfwordsNew->word = newWord;
		//�l�̃Z�b�g
		idfwordsNew->df = dfVol;
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
	//�@���@idf�̃��X�g�쐬�@��

	//�@��	tf�̃��X�g�쐬�@��
	for (filenum = 1; filenum <= 100; filenum++)
	{
		sprintf(fileName, "%s%03d%s", "..\\data\\tf_new_", filenum, ".txt");
		if ((file = fopen(fileName, "r")) == NULL)
		{
			printf("�ǂݍ��݃t�@�C�����J���܂���\n");
			exit(EXIT_FAILURE);
		}

		sprintf(outputFileName, "%s%03d%s", "..\\data\\tfidf_new_", filenum, ".txt");
		if ((outputfile = fopen(outputFileName, "w")) == NULL)
		{
			printf("�������݃t�@�C�����J���܂���B\n");
			exit(EXIT_FAILURE);
		}

		char word[256];
		int tfVol = 0;
		float tfWeight = 0;
		tfwordsList = NULL;


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
			//�\���̂ɒl��ۑ�����
			tfwordsNew->tf = tfVol;
			tfwordsNew->tfWeight = tfWeight;
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

		//�@���@tf��idf�̊|�����킹�@��
		tfwordsThis = tfwordsList;
		while (1) {
			idfwordsThis = idfwordsList;
			while (1)
			{
				if (strcmp(tfwordsThis->word, idfwordsThis->word) == 0)//���o������ƃ��X�g���̌ꂪ��v�����Ƃ�
				{
					break;
				}
				else//���̃A�h���X������̂ŁC���̃����N�ɐi��
				{
					idfwordsThis = idfwordsThis->nextAddr;
				}
			}

			tfwordsThis->tfidfWeight = (tfwordsThis->tfWeight) * (idfwordsThis->idfWeight);

			if (tfwordsThis->nextAddr == NULL)
				break;
			else
				tfwordsThis = tfwordsThis->nextAddr;
		}
		//�@���@tf��idf�̊|�����킹�@��

		//�@���@�\�[�g�@��
		//���X�g�̒����𑪒�
		tfwordsThis = tfwordsList;
		while (1)
		{
			listLength++;
			if (tfwordsThis->nextAddr == NULL)
			{
				//���X�g�̖����ł���̂ŁC���[�v�𔲂���
				break;
			}
			else
			{
				//�w�������A�h���X�����ɐi�߂�
				tfwordsThis = tfwordsThis->nextAddr;
			}
		}

		for (int i = 0; i < listLength; i++)
		{
			if (tfwordsList != NULL)
			{
				tfwordsThis = tfwordsList;
				wordsPre = NULL;

				while (1)
				{
					if (tfwordsThis->nextAddr != NULL)
					{
						if ((tfwordsThis->tfidfWeight) < (tfwordsThis->nextAddr->tfidfWeight))
						{
							wordsTemp = tfwordsThis;     //���������̃A�h���X��ۊ�
							tfwordsThis = wordsTemp->nextAddr;
							wordsTemp->nextAddr = tfwordsThis->nextAddr;
							tfwordsThis->nextAddr = wordsTemp;

							if (wordsPre == NULL)//��������I�u�W�F�N�g���擪��������
							{
								tfwordsList = tfwordsThis;
							}
							else
							{
								//������ɏ�ʂɂȂ�I�u�W�F�N�g��O�̃I�u�W�F�N�g�̃A�h���X�ɓo�^����
								wordsPre->nextAddr = tfwordsThis;
							}
							wordsPre = tfwordsThis;     //�ЂƂO�̍\���̂̃A�h���X���X�V����
							tfwordsThis = tfwordsThis->nextAddr;     //�����ŎQ�Ƃ���\���̂̃A�h���X���X�V����
						}
						else
						{
							wordsPre = tfwordsThis;     //�ЂƂO�̍\���̂̃A�h���X���X�V����
							tfwordsThis = tfwordsThis->nextAddr;     //�����ŎQ�Ƃ���\���̂̃A�h���X���X�V����
						}
					}
					else
					{
						break;
					}
				}
			}
		}
		//�@���@�\�[�g�@��

		//�@���@�t�@�C���������݁@��
		if (tfwordsList->word != NULL)
		{
			tfwordsThis = tfwordsList;
			while (1)
			{
				fprintf(outputfile, "%s\t%d\t%f\t%f\n", tfwordsThis->word, tfwordsThis->tf, tfwordsThis->tfWeight, tfwordsThis->tfidfWeight);
				if (tfwordsThis->nextAddr == NULL)
				{
					//���X�g�̖����ł���̂ŁC���[�v�𔲂���
					break;
				}
				else
				{
					//�w�������A�h���X�����ɐi�߂�
					tfwordsThis = tfwordsThis->nextAddr;
				}
			}
		}
		//�@���@�t�@�C���������݁@��

		//�@���@���X�g�̈�̉���@��
		if (tfwordsList != NULL)
		{
			tfwordsThis = tfwordsList;

			while (tfwordsThis != NULL)
			{
				wordsNext = tfwordsThis->nextAddr;     //(1)���̍\���̂̃A�h���X�����o��
				free(tfwordsThis->word);     //(2)���w���Ă���\���̂Ɋ܂܂���̃��������������
				free(tfwordsThis);     //(3)���w���Ă���\���̂��������
				tfwordsThis = wordsNext;     //(4)���w���Ă���\���̂̃A�h���X���X�V����
			}
		}
		//�@���@���X�g�̈�̉���@��
	}
	//�@���@tf�̃��X�g�쐬�@��
	return 0;
}