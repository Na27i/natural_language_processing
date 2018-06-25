#define _CRT_SECURE_NO_WARNINGS
#define NUM 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct wordStruct {
	char *word;     //�P��
	int count;     //�o�Ă�����
	struct wordStruct *nextAddr;     //���̍\���̂̃A�h���X
}wordStruct;

struct wordStruct *wordsTemp;     //����ւ��Ȃǂ̎��Ɉꎞ�I�ɍ\���̂̃A�h���X������
struct wordStruct *wordsPre;     //wordsThis �̃A�h���X�������Ă���\���̂̃A�h���X������(�����p�̉��u����)
struct wordStruct *wordsNext;    //���w���Ă���\���̂̎��̃A�h���X������

int main(int argc, char **argv)
{
	FILE *file;     //�t�@�C���|�C���^(�ǂ݂���)
	FILE *outputFile;     //�t�@�C���|�C���^(��������)
	char fileName[512] = "";    //�t�@�C�������i�[���邽�߂̔z��
	char outputFileName[512] = "";    //�t�@�C�������i�[���邽�߂̔z��

	sprintf(outputFileName, "%sdf_new%s", "..\\data\\", ".txt");
	if ((outputFile = fopen(outputFileName, "w")) == NULL)
	{
		printf("�������݃t�@�C�����J���܂���B\n");
		exit(EXIT_FAILURE);
	}

	int foundFlag;
	wordStruct *wordsThis;    //���X�g�\���ō��w���Ă���\���̂̃A�h���X
	wordStruct *wordsList = NULL;
	wordStruct *wordsNew;

	char *newWord;

	int filenum;     //�t�@�C���ԍ�

	int listLength = 0;     //���X�g�̒���

	for (filenum = 1; filenum <= 100; filenum++)
	{
		sprintf(fileName, "%s%03d%s", "..\\data\\tf_new_", filenum, ".txt");
		if ((file = fopen(fileName, "r")) == NULL)
		{
			printf("�ǂݍ��݃t�@�C�����J���܂���\n");
			exit(EXIT_FAILURE);
		}

		char word[256];
		float tfVol = 0;
		float tfWeight = 0;

		while (fscanf(file, "%s\t%f\t%f\n", word, &tfVol, &tfWeight) != EOF)
		{
			printf("%03d:%s\n", filenum, word);
			newWord = (char *)malloc(sizeof(char) * (strlen(word) + 1));
			strcpy(newWord, word);

			foundFlag = 0;
			wordsThis = wordsList;
			while (1)
			{
				if (wordsThis == NULL)//�܂� 1 ���̓o�^���Ȃ����
				{
					break;
				}
				else if (strcmp(wordsThis->word, newWord) == 0)//���o������ƃ��X�g���̌ꂪ��v�����Ƃ�
				{
					foundFlag = 1;
					break;
				}
				//���X�g�����ǂ邽�߂̃`�F�b�N
				if (wordsThis->nextAddr == NULL)//�������w���Ă���Ƃ�
				{
					break;
				}
				else//���̃A�h���X������̂ŁC���̃����N�ɐi��
				{
					wordsThis = wordsThis->nextAddr;
				}
			}

			if (foundFlag == 1)//���X�g�Ɍꂪ�������̂ŁC���̌�̃J�E���g�𑝂₷
			{
				wordsThis->count++;
				free(newWord);//���̌�͂��łɃ��X�g�ɂ���̂ŁC�Ƃ��Ă����K�v���Ȃ��̂ŉ������
			}
			else//���X�g�ɂȂ������̂ŁC�P���ǉ�����D
			{
				//�V���Ȍ��ۑ����邽�߂̐V���ȍ\���̗̈���m�ۂ���imalloc�j
				wordsNew = (wordStruct *)malloc(sizeof(wordStruct) * 1);
				//�\���̂Ɍ��ۑ�����
				wordsNew->word = newWord;
				//��̏o���񐔂� 1 �ɃZ�b�g����
				wordsNew->count = 1;
				//���̍\���̂������ɂȂ�̂ŁC���̎��̍\���̂̃A�h���X�� NULL �ɃZ�b�g����
				wordsNew->nextAddr = NULL;

				//���ꂪ���߂Ẵ��X�g�ł��鎞
				if (wordsList == NULL)
				{
					//���X�g�̐擪���w���|�C���^�ϐ��ɁC�V����������\���̂̃A�h���X��������
					wordsList = wordsNew;
				}
				else//���łɂ��郊�X�g�ɒǉ�����Ƃ�
				{
					//�����̍\���̂̃A�h���X�����ɁC�V����������\���̂̃A�h���X��������
					wordsThis->nextAddr = wordsNew;
				}
			}
		}
	}

	//���X�g�̒����𑪒�
	while (1)
	{
		listLength++;
		if (wordsThis->nextAddr == NULL)
		{
			//���X�g�̖����ł���̂ŁC���[�v�𔲂���
			break;
		}
		else
		{
			//�w�������A�h���X�����ɐi�߂�
			wordsThis = wordsThis->nextAddr;
		}
	}

	//�@���@�\�[�g
	for (int i = 0; i < listLength; i++)
	{
		if (wordsList != NULL)
		{
			wordsThis = wordsList;
			wordsPre = NULL;

			while (1)
			{
				if (wordsThis->nextAddr != NULL)
				{
					if (wordsThis->count < wordsThis->nextAddr->count)
					{
						wordsTemp = wordsThis;     //���������̃A�h���X��ۊ�
						wordsThis = wordsTemp->nextAddr;
						wordsTemp->nextAddr = wordsThis->nextAddr;
						wordsThis->nextAddr = wordsTemp;

						if (wordsPre == NULL)//��������I�u�W�F�N�g���擪��������
						{
							wordsList = wordsThis;
						}
						else
						{
							//������ɏ�ʂɂȂ�I�u�W�F�N�g��O�̃I�u�W�F�N�g�̃A�h���X�ɓo�^����
							wordsPre->nextAddr = wordsThis;
						}
						wordsPre = wordsThis;     //�ЂƂO�̍\���̂̃A�h���X���X�V����
						wordsThis = wordsThis->nextAddr;     //�����ŎQ�Ƃ���\���̂̃A�h���X���X�V����

					}
					else
					{
						wordsPre = wordsThis;     //�ЂƂO�̍\���̂̃A�h���X���X�V����
						wordsThis = wordsThis->nextAddr;     //�����ŎQ�Ƃ���\���̂̃A�h���X���X�V����
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	//�@���@�\�[�g

	//�@���@��������
	if (wordsList->word != NULL)
	{
		wordsThis = wordsList;
		while (1)
		{
			fprintf(outputFile, "%s\t%d\t%f\n", wordsThis->word, wordsThis->count, (log10(NUM / wordsThis->count)) + 1);
			if (wordsThis->nextAddr == NULL)
			{
				//���X�g�̖����ł���̂ŁC���[�v�𔲂���
				break;
			}
			else
			{
				//�w�������A�h���X�����ɐi�߂�
				wordsThis = wordsThis->nextAddr;
			}
		}
	}
	// ���@��������

	return 0;
}