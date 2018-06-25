#define _CRT_SECURE_NO_WARNINGS

#include "mecab.h"
#include <stdio.h>
#include <stdlib.h>

#define CHECK(eval) if (! eval) { \
    fprintf (stderr, "Exception:%s\n", mecab_strerror (mecab)); \
    mecab_destroy(mecab); \
    return -1; }


typedef struct wordStruct {
	char *word;     //�P��
	int count;     //�o�Ă�����
	struct wordStruct *nextAddr;     //���̍\���̂̃A�h���X
}wordStruct;

struct wordStruct *wordsTemp;     //����ւ��Ȃǂ̎��Ɉꎞ�I�ɍ\���̂̃A�h���X������
struct wordStruct *wordsPre;     //wordsThis �̃A�h���X�������Ă���\���̂̃A�h���X������(�����p�̉��u����)
struct wordStruct *wordsNext;    //���w���Ă���\���̂̎��̃A�h���X������


int main(int argc, char **argv) {
	FILE *file;     //�t�@�C���|�C���^
	int fileSize;     //�t�@�C���̃T�C�Y
	char fileName[512] = "";    //�t�@�C�������i�[���邽�߂̔z��

	char *string;     //��������i�[���邽�߂̔z��
	char ch;     //roop�p
	int i;     //roop�p

	char *word;     //�t�@�C��������o����������

	int foundFlag = 0;     //
	wordStruct *wordsThis;    //���X�g�\���ō��w���Ă���\���̂̃A�h���X
	wordStruct *wordsList;     //
	wordStruct *wordsNew;

	mecab_t *mecab;
	const mecab_node_t *node;

	int filenum;

	for (filenum = 1; filenum <= 100; filenum++)
	{
		sprintf(fileName, "%s%03d%s", "..\\data\\", filenum, ".txt");
		if ((file = fopen(fileName, "r")) == NULL)
		{
			printf("file open error!!\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			fseek(file, 0, SEEK_END);     //�t�@�C���|�C���^�𖖔��Ɉړ�������
			fileSize = ftell(file);     //�t�@�C���T�C�Y���擾����
			printf("%s �t�@�C���̃T�C�Y:%d�o�C�g\n", fileName, fileSize);
			fseek(file, 0, SEEK_SET);     //�t�@�C���|�C���^��擪�ɖ߂�
		}



		FILE *outputFile;
		char outputFileName[32] = "";
		sprintf(outputFileName, "%s%03d%s", "..\\data\\tf_", filenum, ".txt");
		if ((outputFile = fopen(outputFileName, "w")) == NULL)
		{
			printf("�t�@�C�����J���܂���", outputFileName);
			exit;
		}



		wordsList = NULL;

		string = (char *)malloc(sizeof(char) * (fileSize + 1));
		for (i = 0; (ch = fgetc(file)) != -1; i++)
		{
			string[i] = ch;
		}
		string[i] = '\0';

		mecab = mecab_new(argc, argv);
		CHECK(mecab);
		// Gets node object
		node = mecab_sparse_tonode(mecab, string);
		CHECK(node);
		for (; node; node = node->next)
		{
			if (node->stat == MECAB_NOR_NODE || node->stat == MECAB_UNK_NODE)
			{
				fwrite(node->surface, sizeof(char), node->length, stdout);
				printf("\t%s\n", node->feature);
				word = (char *)malloc(sizeof(char) * (node->length + 1));
				strncpy(word, node->surface, node->length);
				word[node->length] = '\0';

				foundFlag = 0;
				wordsThis = wordsList;
				while (1)
				{
					if (wordsThis == NULL)//�܂� 1 ���̓o�^���Ȃ����
					{
						break;
					}
					else if (strcmp(wordsThis->word, word) == 0)//���o������ƃ��X�g���̌ꂪ��v�����Ƃ�
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
					free(word);//���̌�͂��łɃ��X�g�ɂ���̂ŁC�Ƃ��Ă����K�v���Ȃ��̂ŉ������
				}
				else//���X�g�ɂȂ������̂ŁC�P���ǉ�����D
				{
					//�V���Ȍ��ۑ����邽�߂̐V���ȍ\���̗̈���m�ۂ���imalloc�j
					wordsNew = (wordStruct *)malloc(sizeof(wordStruct) * 1);
					//�\���̂Ɍ��ۑ�����
					wordsNew->word = word;
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
		mecab_destroy(mecab);

		int listLength = 0;
		int wordsVolume = 0;

		//���X�g�̒����𑪒�
		while (1)
		{
			listLength++;
			wordsVolume += wordsThis->count;
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

		listLength = 0;

		if (wordsList->word != NULL)
		{
			wordsThis = wordsList;
			while (1)
			{
				printf("%s:%d:%f\n", wordsThis->word, wordsThis->count, (float)(wordsThis->count) / wordsVolume);
				fprintf(outputFile, "%s\t%d\t%f\n", wordsThis->word, wordsThis->count, (float)(wordsThis->count) / wordsVolume);
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

		//���X�g�̈���������
		if (wordsList != NULL)
		{
			wordsThis = wordsList;

			while (wordsThis != NULL)
			{
				wordsNext = wordsThis->nextAddr;     //(1)���̍\���̂̃A�h���X�����o��
				free(wordsThis->word);     //(2)���w���Ă���\���̂Ɋ܂܂���̃��������������
				free(wordsThis);     //(3)���w���Ă���\���̂��������
				wordsThis = wordsNext;     //(4)���w���Ă���\���̂̃A�h���X���X�V����
			}
		}
	}
	return 0;
}
