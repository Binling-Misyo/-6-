#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
//������Ŀ�����ֵ����ѡ����ѡ ,�жϣ�����û��� 
#define scodan 3 
#define scoduo 5 
#define scopd 2 
#define maxusers 100
//�������ݱ����ļ�,�����ǽ�ʦ�˺ţ�ѧ���˺ţ���⣬������ ,Ĭ��ѧ�����룬����������� 
#define teaaccf "teacher.dat" 
#define stuaccf "student.dat" 
#define tkf "tm.dat"
#define resf "result.dat"
#define mpw "123"
#define qmax 50
// ����Ĭ�����߲�����Ŀ����
int Sq = 3;
int Mq = 3;
int Jq = 3;
struct user{//�˺Žṹ�������ʦѧ���������˺������� 
	char name[30];
	char acc[30];
	char pw[30];
	char type[8];
}; 
struct qqq{//����ṹ����������ɣ�����ѡ�� ����Ŀ��� 
	char txt[2000];
	char choice[20][100];
	int chonum;
	int ans[20];//�� 
	char id[5];//Ψһ��Ŀ��� 
	char type;//a����ѡ��b�����ѡ��c�����ж��� 
};
struct res{//�����¼�ṹ���������ߴ����¼ 
	char name[30];
	char tilist[50][6];//��Ŀ����б� 
	int tinum;//��Ŀ�� 
	int torf[50];//��ȷ��� 
	int sco;//����
	int man;//���ַ��� 
	char date[20];//���� 
}; 
int GUI();//��GUI��� 
void signACC();//��ʦ��ѧ���˺�ע�ắ�� 
	int acccfpd(const char* chs);//�˺��ظ��жϣ��ظ�����1�����ظ�����0 
int dengluGUI();//��½����GUI�Լ�ѧ���ͽ�ʦGUI������� ������0��ʾ�˳�ϵͳ 
	int accyzpd(struct user *acc); //��֤�˺ţ����󷵻�1����ȷ����0
	int stuGUI(struct user *acc);//ѧ������GUI�Լ�ѧ�����ܺ������ ������0��ʾ�˳�ϵͳ 
		void stupwset(struct user *acc);//ѧ���˸��������޸Ĺ��� 
		void exam(struct user* acc);//ѧ�������ߴ��⹦��
			void tkzuhe(struct qqq* examQuestions);//������������Ϻ���
			void dlcho(struct qqq* qq);//����ѡ��˳�� 
			void update(char name[], struct qqq* Ex, int* torf, int score, int man);//ѧ�������¼���º��� 
		void stulook(struct user* acc);//��ѯѧ�����˴������
	int teaGUI(struct user *acc);//��ʦ����GUI�Լ�ѧ�����ܺ�����ڣ�����0��ʾ�˳�ϵͳ 
		int tikuGUI();//��ʦ��������GUI���� 
			void tikuadd();//������� 
				void tiid(char *id, char questionType);//��������Ŀ���
			void tikudel();//��Ŀɾ�� 
			void tikuset();//��Ŀ�޸�
			void tikulook();//���鿴
				void duoxtprint(struct qqq question);//��ѡ���ӡ
				void danxtprint(struct qqq question);//��ѡ���ӡ
				void pdtprint(struct qqq question);//�ж����ӡ
		void teapwset(struct user *acc);//��ʦ�˸��������޸Ĺ��� 
		int stuCTRLGUI();//��ʦ��ѧ�����ܹ���ϵͳ��� ������0��ʾ�˳�ϵͳ 
			void stuCTRLadd();//��ʦ��ѧ����Ϣ�������ѧ������ 
			void stuCTRLset(int mode);//��ʦ��ѧ����Ϣ�����޸�ѧ����Ϣ����
			void stuCTRLdel();//��ʦ��ѧ����Ϣ����ɾ��ѧ������
			void stuCTRLlook();//ѧ���˺ſ�鿴 
		void tealook();//��ʦ�˲�ѯѧ������������
			void relookalone();//��ѯ������
			void relookall();//��ѯ����
void tip()//����ע�� 
{
	printf("\n				       ������ע�͡�\n\n");
	printf("\n	*�ڱ������˺�ϵͳ�У��˺ź���Ψһ�ԣ��������ԣ���������������ظ�\n");
	printf("\n	*�������һ��ʹ��ʱ����ע���ʦ��ѧ�����͵������˺�\n");
	printf("\n	*Ϊ�������������������ÿһ���ⶼ����ר����ţ�ר�����������\n");
	printf("\n  ��Ŀʱ�Զ����ɣ���ʽΪ������ĸ��+������ǰ��0����λ���֣���������\n");
	printf("\n  ��ĸ������Ŀ���ͣ�A����ѡ�⣬B�����ѡ�⣬C�����ж��⣻���ִ�\n");
	printf("\n  �����Ǹ�������Ŀ�ĵڼ����⣬��B003��ָ��������������ж�ѡ��,\n");
	printf("\n  ɾ����Ŀʱ��������Ŀ��Ų��䣬���޸���Ŀʱ����Ž������δ��ǰ���������Ŀ���+1��\n");
	printf("\n	*��ǰѧ���˺�����Ĭ������Ϊ %s\n",mpw);
	printf("\n	*���ϵͳ��һ�������֧��20��ѡ����ɿ���ʱ��������������ȡ\n\n  �̶��������⣬ĿǰĬ�Ͽ������Ϊ %d����ѡ��+%d����ѡ��+%d���ж���\n",Sq,Mq,Jq);
	printf("\n	*��ǰ��Ŀ��ֵΪ��ѡ��%d�֣���ѡ��%d�֣��ж���%d��\n",scodan,scoduo,scopd); 
	printf("\n  �ҵ�ѡ��Ͷ�ѡ���ѡ��˳��ᱻ�������\n"); 
	printf("\n	*������......\n");
	char s=getchar();
	system("cls");
}
void SetColor(int ForgC) //UI�����ɫ���ú��� 
{
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}
int szpd(int a[], int b[]) // �����������Ƚ�a[]��b[]�Ƿ��Ǻ�����ͬԪ�ص����飬���ڿ���ģ�����÷�
{
    int ac[20] = {0}, bc[20] = {0};
    for (int i = 0; a[i] != 0; i++) ++ac[a[i]];
    for (int i = 0; b[i] != 0; i++) ++bc[b[i]];
    for (int i = 0; i < 20; i++) {
        if (ac[i] != bc[i])
            return 0;
    }
    return 1;
}
void shufflePartialArray(void* array, int size, int elemSize) // ������ҽṹ�����麯��
{
    srand(time(NULL));
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp[elemSize];
        memcpy(temp, (char*)array + i * elemSize, elemSize);
        memcpy((char*)array + i * elemSize, (char*)array + j * elemSize, elemSize);
        memcpy((char*)array + j * elemSize, temp, elemSize);
    }
}
void pass()//��������ʾģ�飬���ڳ��򿪷�ά��ʹ�� 
{
	SetColor(8);
	printf("*�ù��ܴ�����");
	SetColor(15);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	SetConsoleTitle("˼��֪ʶѧϰ����ϵͳ���۷�棩");//���ڱ��� 
	SetColor(15);
	while(GUI()!=0);
	return 0;
}
//***************************************************************************************************************************
int GUI() //��GUI���
{
	for(int i=0;i<65;i++)
	{
//		Sleep(100);
        printf("_");
    }
    printf("\n");
	printf("                                                               ||\n");
	SetColor(12);
    printf("            THE ˼��֪ʶѧϰ����ϵͳ���۷�棩                 ");
    SetColor(15);
    printf("||\n");
    printf("                                                               ||\n");
    char n = 65;
    while (n--)
	{
//		Sleep(100);
        printf("^");
    }
    Sleep(5);
    printf("\n��ѡ�����Ĳ���:\n\n");
    Sleep(100);
    printf("    [1]��½\n\n");
    Sleep(100);
    printf("    [2]ע��\n\n");
    Sleep(100);
    printf("    [3]����\n\n");
    Sleep(100);
    printf("    [0]�رճ���\n\n ");
    Sleep(100);
	SetColor(11);
    printf("��������Ӧ����");
    for (int i = 0; i < 3; i++)
	{
        Sleep(10);
        printf(">");
    }
    SetColor(15);
    int m=0;
	do{
		SetColor(11);
		if(m>0) printf("\n\a[!] δ֪ѡ������������>>> ");
		SetColor(15);
		scanf("%[^\n]", &n);
		getchar();
		m++;
	}while(n!='0'&&n!='1'&&n!='2'&&n!='3');
	system("cls");//����ı� 
	if(n=='0') return 0;
	else if(n=='1')
	{
		dengluGUI();
		return 1;
	}
	else if(n=='2')
	{
		signACC();
		return 1;
	}
	else if(n=='3')
	{
		tip();
		return 1;
	}
}
void signACC() //��ʦ��ѧ���˺�ע�ắ��
{
	struct user acc;
	SetColor(11);
	printf("\n|===================[--��---��---ע---��--]\n");
	SetColor(15);
	int mark=0; 
	do{
		if(mark>0) printf("\n\a	����������������룡����\n");
		printf("\n	�������˺���ݣ���ʦ/ѧ����>>> "),scanf("%s",acc.type);
		mark++;
	}while(strcmp(acc.type,"��ʦ")!=0&&strcmp(acc.type,"ѧ��")!=0);
	printf("\n	���������ĳƺ� >>> "),scanf("%s",acc.name);
	printf("\n	�õģ��𾴵�%s\n",acc.name);
	mark=0;
	do{
		if(mark>0) printf("\n	���˺��Ѵ��ڣ����޸ĺ��������룡����\n"); 
		printf("\n	�������봴�����ġ�Ψһ���˺�>>> "),scanf("%s",acc.acc);
		mark++;
	}while(acccfpd(acc.acc));
	printf("\n	���벢ȷ����������>>> "),scanf("%s",acc.pw);
	getchar();
// �����˺���Ϣ���ļ�
    FILE *fp = fopen(strcmp(acc.type,"��ʦ")==0 ? teaaccf : stuaccf, "ab");
    if (fp == NULL)
    {
        printf("\n	�ļ���ʧ�ܣ�ע��ʧ�ܣ�\n");
        return;
    }
    fwrite(&acc, sizeof(struct user), 1, fp); // ���û���Ϣд���ļ�
    fclose(fp); 
    printf("\n	ע��ɹ�������������һ��");
    Sleep(1000);
    system("cls");
}
int acccfpd(const char* chs)
{
	FILE* tfp = fopen(teaaccf, "rb");
	FILE* sfp = fopen(stuaccf, "rb");
	if (tfp == NULL|| sfp == NULL) return 0; // ����ļ������ڣ�˵��û���ظ�
	
	struct user temp;
	while (fread(&temp, sizeof(struct user), 1, tfp) == 1)
	{
	    if (strcmp(temp.acc, chs) == 0)
	    {
	            fclose(tfp);
	            return 1; // �ҵ��ظ��˺�
	    }
    }
    while (fread(&temp, sizeof(struct user), 1, sfp) == 1)
	{
	    if (strcmp(temp.acc, chs) == 0)
	    {
	        fclose(sfp);
	        return 1; // �ҵ��ظ��˺�
	    }
    }
    fclose(tfp);
    fclose(sfp);
    return 0; // û���ҵ��ظ��˺�
}
int dengluGUI()//��½����GUI�Լ�ѧ���ͽ�ʦGUI������� ������0��ʾ�˳�ϵͳ 
{
	struct user acc;
	SetColor(11);
	printf("\n|===================[--��---��---��---¼--]\n");
	SetColor(15);
	int mark=0;
	do{
		if(mark>0) printf("\n\a[!] �˺Ż�����������������룡����\n");
		printf("\n	�����������˺�>>> "),scanf("%s",acc.acc);
		getchar();
		printf("\n	��������������>>> "),scanf("%s",acc.pw);
		getchar();
		mark++;
	}while(accyzpd(&acc));
	system("cls");
	if(strcmp(acc.type,"��ʦ") ==0) while(teaGUI(&acc));
	else while(stuGUI(&acc));
	return 1;
}
int accyzpd(struct user *acc)//��֤�˺ţ����󷵻�1����ȷ����0,ͬʱ���˻����в�ȫ��Ϣ 
{
	FILE* tfp = fopen(teaaccf, "rb");
	FILE* sfp = fopen(stuaccf, "rb");
    if (tfp == NULL||sfp==NULL) // �ļ������ڣ���֤ʧ��
	{
		printf("�ļ������ڣ���֤ʧ��\n");
		return 1;
	}
    struct user temp;
    while (fread(&temp, sizeof(struct user), 1, tfp) == 1)
    {
        if (strcmp(temp.acc, acc->acc) == 0 && strcmp(temp.pw, acc->pw) == 0)
        {
            *acc = temp; // ��ȫ��Ϣ
            fclose(tfp);
            fclose(sfp);
            return 0; // ��֤�ɹ�
        }
    }
    while (fread(&temp, sizeof(struct user), 1, sfp) == 1)
    {
        if (strcmp(temp.acc, acc->acc) == 0 && strcmp(temp.pw, acc->pw) == 0)
        {
            *acc = temp; // ��ȫ��Ϣ
            fclose(tfp);
            fclose(sfp);
            return 0; // ��֤�ɹ�
        }
    }
    fclose(sfp);
    fclose(tfp);
    return 1; // ��֤ʧ�� 	
} 
int stuGUI(struct user *acc)//ѧ������GUI�Լ�ѧ�����ܺ������ ������0��ʾ�˳�ϵͳ 
{
	SetColor(11);
	printf("\n\nQAQ/ ��ӭ %s ͬѧ\n",acc->name);
	SetColor(15);
	printf("\n	[1]���߲���\n");
	printf("\n	[2]�޸ĸ�������\n");
	printf("\n	[3]���˴��������ѯ\n");
	printf("\n	[0]�˳���¼\n");
	int cho,m=0;
	do{
		if(m>0) printf("\n[!] ���ƺ�������ˣ����������룡����\n");
		printf("\n�������Ĳ���>>> ");
		scanf("%d",&cho);
		getchar(); 
	}while(cho!=1&&cho!=2&&cho!=3&&cho!=0);
	system("cls");
	if(cho==0) return 0;
	else if(cho==1) exam(acc); 
	else if(cho==2) stupwset(acc);
	else if(cho==3) stulook(acc);
	else pass();
	return 1;
}
void stupwset(struct user *acc)//ѧ���˸��������޸Ĺ��� 
{
	FILE *fp=fopen(stuaccf,"r+b");
	if (fp == NULL) // �ļ������ڣ���֤ʧ��
	{
		printf("�ļ������ڣ���֤ʧ��\n");
		exit(0);
	}
	struct user temp;
	while (fread(&temp, sizeof(struct user), 1, fp) == 1) if (strcmp(temp.acc, acc->acc) == 0 )
	{
    	break; // �˺�λ�ò�ѯ�ɹ� 	
	}
	fseek(fp,-sizeof(struct user),SEEK_CUR);//�ļ�ָ��Ǩ�����˺�λ��
	SetColor(11);
	printf("\n|===================[--��---��---��---��--]\n");
	SetColor(15);
	int ok=0;
	temp.pw[0]='\0';
	do{
		if(ok) printf("\n	[!] ��������������������룡\n");
		printf("\n	����֤����ԭ����>>> ");
		scanf("%[^\n]s",temp.pw);
		getchar();
		ok++;
	}while(strcmp(temp.pw, acc->pw) != 0);
	printf("\n	����������������>>> ");
	scanf("%[^\n]s",acc->pw);
	fwrite(acc,sizeof(struct user),1,fp);
	fclose(fp);
	printf("	\n�޸ĳɹ�!\n���س����ز˵���");
	scanf("%[^\n]",&temp.acc);
	system("cls");
	getchar();
}
void exam(struct user* acc) //���⺯�� 
{
	int ma=0,zdyex;
	printf("\n�Ƿ�ʹ���Զ��忼��������\n([1] ʹ��,[0]��ʹ��(ʹ��Ĭ�Ͽ�����) )>>> ");
	scanf("%d",&zdyex);
	if(zdyex==1)
	{
		printf("���������뵥ѡ�⣬��ѡ�⣬�ж�������(��������಻����%d)>>> ",qmax);
		scanf("%d %d %d",&Sq,&Mq,&Jq);	
	}while(Sq+Mq+Jq>qmax||Sq+Mq+Jq<1);
	system("cls"); 
    struct qqq Ex[qmax];
    int Exl = Sq + Mq + Jq;
    tkzuhe(Ex);
    for (int i = 0; i < Exl; i++)
	{
        dlcho(&Ex[i]);
    }

    int sco = 0;
    int man = Sq * scodan + Mq * scoduo + Jq * scopd;
    int stuans[qmax][20] = {0};
    int torf[qmax] = {0};

    int ti = 0;
    while (ti < Exl) {
        struct qqq currentQuestion = Ex[ti];
        printf("%d/%d: %s\n", ti + 1, Exl, currentQuestion.txt);
//        for(int i=0;currentQuestion.ans[i]!=0&&i<20;i++) printf(" %d",currentQuestion.ans[i]);    ��ӡ�𰸣����Դ����� 
		printf("\n");
        if (currentQuestion.type == 'a' || currentQuestion.type == 'b') 
		{
            for (int i = 0; i < currentQuestion.chonum; i++) {
                printf("%d. %s\n", i + 1, currentQuestion.choice[i]);
            }
            printf("����𰸣���ѡ�ÿո�ָ�)��");
            int answer;
            int validInput = 0;
            int inputCount = 0;
            char input[100];
            scanf(" %[^\n]", input); // ��ȷ��ȡ��ѡ���
            char* token = strtok(input, " ");
            while (token != NULL)
			{
                answer = atoi(token);
                if (answer < 1 || answer > currentQuestion.chonum) 
				{
					break;
				}
                stuans[ti][inputCount++] = answer;
                token = strtok(NULL, " ");
            }
        }
		else if (currentQuestion.type == 'c')
		{
            printf("1. ��ȷ\n2. ����\n");
            printf("����𰸣�1��2����");
            int answer;
            while (1) {
                scanf("%d", &answer);
                if (answer == 1 || answer == 2)
				{
                    break;
                }
                printf("����������������룺");
            }
            stuans[ti][0] = answer;
        }
        // �жϴ��Ƿ���ȷ
        int correct = 0;
        if (currentQuestion.type == 'a') correct = (stuans[ti][0] == currentQuestion.ans[0]);
		else if (currentQuestion.type == 'b') correct = szpd(currentQuestion.ans, stuans[ti]);
		else if (currentQuestion.type == 'c') correct = (stuans[ti][0] == currentQuestion.ans[0]);
        torf[ti] = correct;
        if (correct) sco += (currentQuestion.type == 'a' ? scodan : (currentQuestion.type == 'b' ? scoduo : scopd));
        ti++;
        if (ti < Exl)
		{
            system("cls"); // ȷ������
        }
    }
    printf("���Խ�������ĵ÷��� %d/%d\n\n��ǰ�����˴����¼�鿴����", sco, man);
    update(acc->name, Ex, torf, sco, man);
    Sleep(2000);
    system("cls");
}
void tkzuhe(struct qqq* examQuestions) //�����ȡ���������Ծ�
{
    FILE* fp = fopen(tkf, "rb");
    struct qqq temp;
    int sc = 0, mc = 0, jc = 0;
    struct qqq sq[qmax], mq[qmax], jq[qmax];

    while (fread(&temp, sizeof(struct qqq), 1, fp) == 1) {
        switch (temp.type) {
            case 'a': 
				for(int i=1;i<20;i++)
				{
					temp.ans[i]=0;
				}
				sq[sc++] = temp;
				break;
            case 'b':
            {
            	int p=temp.chonum;
				for(int i=3;i<20;i++) if(temp.ans[i]>temp.chonum||temp.ans[i]==0)
				{
					p=i;
					break;
				}
				for(int i=p;i<20;i++) temp.ans[i]=0;
				mq[mc++] = temp; 
				break;
			}
            case 'c':  
				for(int i=1;i<20;i++)
				{
					temp.ans[i]=0;
				}
				jq[jc++] = temp;
				break;
        }
    }
    fclose(fp);

    if (sc < Sq || mc < Mq || jc < Jq) {
        printf("\n�����Ŀ���㣬��������ʧ�ܣ�\n");
        Sleep(2000);
        exit(0);
    }

    shufflePartialArray(sq, sc, sizeof(struct qqq));
    shufflePartialArray(mq, mc, sizeof(struct qqq));
    shufflePartialArray(jq, jc, sizeof(struct qqq));

    int Ec = 0;
    for (int i = 0; i < Sq; i++) examQuestions[Ec++] = sq[i];
    for (int i = 0; i < Mq; i++) examQuestions[Ec++] = mq[i];
    for (int i = 0; i < Jq; i++) examQuestions[Ec++] = jq[i];
}
void dlcho(struct qqq *qq) // ����ѡ��˳����������ȷ����ѡ��λ�ñ仯
{
    if (qq->type == 'c') return;
    int n = qq->chonum;
    int old_to_new[20];   // ����������������ӳ��
    // ��ʼ�� old_to_new ����Ϊԭʼ˳��
    for (int i = 0; i < n; i++) 
	{
        old_to_new[i] = i;
    }
    // ʹ��Knuthϴ���㷨����ѡ��
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);  // ���ѡ��Ҫ������Ԫ��

        // ����ѡ��
        char temp[100];
        strcpy(temp, qq->choice[i]);
        strcpy(qq->choice[i], qq->choice[j]);
        strcpy(qq->choice[j], temp);

        // ���� old_to_new ����
        // ���� old_to_new[i] �� old_to_new[j]
        int swap = old_to_new[i];
        old_to_new[i] = old_to_new[j];
        old_to_new[j] = swap;
    }

    // �����������е�����
    for (int i = 0; qq->ans[i] != 0; i++) {
        int old_index = qq->ans[i]; // ԭ���е�ѡ��������1-based��
        if (old_index >= 1 && old_index <= n) { // ԭ������Ч
            // �ҵ� old_to_new �����ж�Ӧ�ľ�����
            int new_idx = -1;
            for (int k = 0; k < n; k++) {
                if (old_to_new[k] == old_index - 1) {  // ԭ����ת��Ϊ 0-based
                    new_idx = k;
                    break;
                }
            }
            if (new_idx != -1)
			{
                qq->ans[i] = new_idx + 1;  // ����Ϊ��������1-based��
            }
        }
    }
}
void update(char name[], struct qqq* Ex, int* torf, int score, int man) //��������¼���� 
{
    struct res result;
    strcpy(result.name, name);
    result.sco = score;
    result.man = man;
    time_t t = time(NULL);//��ȡ��ǰʱ��� 
    struct tm* tm = localtime(&t);//ʱ���ת�� 
    strftime(result.date, sizeof(result.date), "%Y-%m-%d-%H:%M", tm);//��ʽ������ʱ��� 
    result.tinum = Sq + Mq + Jq;
    for (int i = 0; i < result.tinum; i++) {
        strcpy(result.tilist[i], Ex[i].id);
        result.torf[i] = torf[i];
    }

    FILE* fp = fopen(resf, "ab");
    if (fp == NULL) {
        printf("�ļ���ʧ�ܣ��޷���������¼��\n");
        return;
    }
    fwrite(&result, sizeof(struct res), 1, fp);
    fclose(fp);
}
void stulook(struct user* acc) //ѧ�������¼��ѯ���� 
{
    FILE* fp = fopen(resf, "rb");
    if (fp == NULL) {
        printf("����ļ���ʧ�ܣ�\n");
        return;
    }

    struct res temp;
    char name[30];
    strcpy(name, acc->name); // �Զ�ʹ�õ�ǰѧ������

    while (fread(&temp, sizeof(struct res), 1, fp) == 1) {
        if (strcmp(temp.name, name) == 0) {
            printf("============================================================\n");
            printf("������%s\n", temp.name);
            printf("������%d/%d\n", temp.sco, temp.man);
            printf("���������\n");
            for (int i = 0; i < temp.tinum; i++) {
                printf("��Ŀ��ţ�%s�����������%s\n", temp.tilist[i], temp.torf[i] ? "��ȷ" : "����");
            }
            printf("�������ڣ�%s\n",temp.date);
        }
    }

    printf("\n�����������>>> ");
    getchar();
    system("cls");
    fclose(fp);
}
int teaGUI(struct user *acc)//��ʦ����GUI�Լ�ѧ�����ܺ�����ڣ�����0��ʾ�˳�ϵͳ 
{
	SetColor(11);
	printf("\n\n0w0/ ��ӭ %s ��ʦ\n",acc->name);
	SetColor(15);
	printf("\n	[1]������\n");
	printf("\n	[2]�޸ĸ�������\n");
	printf("\n	[3]����ѧ���˺�\n");
	printf("\n	[4]ѧ�����������ѯ\n");
	printf("\n	[0]�˳���¼\n");
	int cho,m=0;
	do{
		if(m>0) printf("\n[!] ���ƺ�������ˣ����������룡����\n");
		printf("\n�������Ĳ���>>> ");
		scanf("%d",&cho);
		getchar();
		m++;
	}while(cho!=1&&cho!=2&&cho!=3&&cho!=0&&cho!=4);
	system("cls"); 
	if(cho==0) return 0;
	else if(cho==1) while(tikuGUI());
	else if(cho==2) teapwset(acc);
	else if(cho==3) while(stuCTRLGUI());
	else if(cho==4) tealook();
	else pass();
	return 1;
}
int tikuGUI()//��ʦ��������GUI���� 
{
	SetColor(11);
	printf("\n|===================[--��---��---��---��--]\n");
	SetColor(15);
	printf("\n	[1]��������\n");
	printf("\n	[2]�����޸�\n");
	printf("\n	[3]����ɾ��\n");
	printf("\n	[4]����鿴\n");
	printf("\n	[0]����\n");
	int cho,m=0;
	do{
	if(m>0) printf("\n[!] ���ƺ�������ˣ����������룡����\n");
	printf("\n�������Ĳ���>>> ");
	scanf("%d",&cho);
	getchar();
	m++; 
	}while(cho!=1&&cho!=2&&cho!=3&&cho!=0&&cho!=4);
	system("cls");
	if(cho==0) return 0;
	else if(cho==1) tikuadd();
	else if(cho==2) tikuset();
	else if(cho==3) tikudel();
	else if(cho==4) tikulook();
	else pass();
}
void tikuadd() // ��ʦ��������亯��
{
    SetColor(11);
    printf("\n|===================[--��---��---��---��--]\n");
    SetColor(15);

    struct qqq question;
	for(int i=0;i<20;i++) question.ans[i]=0;
    int mark = 0;
    do {
        if (mark > 0) printf("\n    ����������������룡����\n");
        printf("\n    ��������Ŀ���ͣ�a: ��ѡ��b: ��ѡ��c: �жϣ�>>> ");
        scanf(" %c", &question.type);
        mark++;
    } while (question.type != 'a' && question.type != 'b' && question.type != 'c');

    // ��������Ŀ���
    tiid(question.id, question.type);

    printf("\n    �������������>>> ");
    scanf(" %[^\n]", question.txt);

    if (question.type == 'c') {
        question.chonum = 2;
        strcpy(question.choice[0], "��ȷ");
        strcpy(question.choice[1], "����");
    } else {
        do {
            printf("\n    ������ѡ��������������20��>>> ");
            scanf("%d", &question.chonum);
            if (question.chonum <= 0 || question.chonum > 20) {
                printf("\n    ѡ����������������������룡����\n");
            }
        } while (question.chonum <= 0 || question.chonum > 20);

        for (int i = 0; i < question.chonum; i++) {
            printf("\n    ������� %d ��ѡ������>>> ", i + 1);
            scanf(" %[^\n]", question.choice[i]);
        }
    }

    int ansCount = 0;
    if (question.type == 'a') {
        do {
            printf("\n    ��������ȷ�𰸵�ѡ����ţ���1��ʼ��>>> ");
            scanf("%d", &question.ans[0]);
            if (question.ans[0] < 1 || question.ans[0] > question.chonum) {
                printf("\n    ���������������������룡����\n");
            }
        } while (question.ans[0] < 1 || question.ans[0] > question.chonum);
        ansCount = 1;
    } else if (question.type == 'b') {
        printf("\n    ��������ȷ�𰸵�ѡ����ţ��Կո�ָ������� -1 ������>>> ");
        int ans;
        ansCount = 0;
        while (1) {
            scanf("%d", &ans);
            if (ans == -1) break;
            if (ans < 1 || ans > question.chonum) {
                printf("\n    ���������������������룡����\n");
                continue;
            }
            question.ans[ansCount++] = ans;
        }
    } else {
        do {
            printf("\n    ��������ȷ�𰸵�ѡ����ţ�1: ��ȷ��2: ����>>> ");
            scanf("%d", &question.ans[0]);
            if (question.ans[0] < 1 || question.ans[0] > 2) {
                printf("\n    ���������������������룡����\n");
            }
        } while (question.ans[0] < 1 || question.ans[0] > 2);
        ansCount = 1;
    }

    // ������Ŀ��Ϣ���ļ�
    FILE *fp = fopen(tkf, "ab");
    if (fp == NULL) {
        printf("\n    �ļ���ʧ�ܣ���Ŀ���ʧ�ܣ�\n");
        return;
    }
    fwrite(&question, sizeof(struct qqq), 1, fp);
    fclose(fp);

    printf("\n    ��Ŀ��ӳɹ�������������һ��");
    Sleep(1000);
    system("cls");
}
void tiid(char *id, char qType) // ��������Ŀ���
{
    FILE *fp = fopen(tkf, "rb");
    if (fp == NULL) {
        // ����ļ������ڣ�˵���ǵ�һ����
        id[0] = (qType == 'a') ? 'A' : (qType == 'b') ? 'B' : 'C';
        id[1] = '0';
        id[2] = '0';
        id[3] = '1';
        id[4] = '\0';
        return;
    }
    struct qqq question;
    int maxNum = 0;
    while (fread(&question, sizeof(struct qqq), 1, fp) == 1) {
        if ((qType == 'a' && question.id[0] == 'A') ||
            (qType == 'b' && question.id[0] == 'B') ||
            (qType == 'c' && question.id[0] == 'C')) 
			{
            int num = atoi(question.id + 1);
            if (num > maxNum) 
			{
                maxNum = num;
            }
        }
    }
    fclose(fp);
    id[0] = (qType == 'a') ? 'A' : (qType == 'b') ? 'B' : 'C';
    sprintf(id + 1, "%03d", maxNum + 1);//id+1 => &id[1]
} 
void tikudel() // ��Ŀɾ������
{
    SetColor(11);
    printf("\n|===================[--��---Ŀ---ɾ---��--]\n");
    SetColor(15);

    char id[5];
    printf("\n    ������Ҫɾ������Ŀ���>>> ");
    scanf("%s", id);

    FILE *fp = fopen(tkf, "rb");
    if (fp == NULL) {
        printf("\n    �ļ���ʧ�ܣ�ɾ��ʧ�ܣ�\n");
        return;
    }

    struct qqq questions[maxusers];
    int count = 0;
    int found = 0;
    while(fread(&questions[count], sizeof(struct qqq), 1, fp) == 1)//���Ҳ��������� 
	{
        if (strcmp(questions[count].id, id) == 0)
		{
            found = 1;
        } else {
            count++;
        }
    }
    fclose(fp);
    if (!found) {
        printf("\n    δ�ҵ�����Ŀ��ţ�ɾ��ʧ�ܣ�\n");
        Sleep(1000);
        system("cls");
        return;
    }

    fp = fopen(tkf, "wb");
    if (fp == NULL) {
        printf("\n    �ļ���ʧ�ܣ�ɾ��ʧ�ܣ�\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fwrite(&questions[i], sizeof(struct qqq), 1, fp);
    }
    fclose(fp);

    printf("\n    ��Ŀɾ���ɹ�������������һ��");
    Sleep(1000);
    system("cls");
}
void tikuset() // ��Ŀ�޸ĺ���
{
    SetColor(11);
    printf("\n|===================[--��---Ŀ---��---��--]\n");
    SetColor(15);

    char id[5];
    printf("\n    ������Ҫ�޸ĵ���Ŀ���>>> ");
    scanf("%s", id);

    FILE *fp = fopen(tkf, "rb+");
    if (fp == NULL) {
        printf("\n    �ļ���ʧ�ܣ��޸�ʧ�ܣ�\n");
        return;
    }

    struct qqq question;
    int found = 0;
    long pos;
    while (fread(&question, sizeof(struct qqq), 1, fp) == 1) {
        if (strcmp(question.id, id) == 0) {
            found = 1;
            pos = ftell(fp) - sizeof(struct qqq);
            break;
        }
    }

    if (!found) {
        printf("\n    δ�ҵ�����Ŀ��ţ��޸�ʧ�ܣ�\n");
        fclose(fp);
        Sleep(1000);
        system("cls");
        return;
    }

    int mark = 0;
    do {
        if (mark > 0) printf("\n    ����������������룡����\n");
        printf("\n    �������µ���Ŀ���ͣ�a: ��ѡ��b: ��ѡ��c: �жϣ�>>> ");
        scanf(" %c", &question.type);
        mark++;
    } while (question.type != 'a' && question.type != 'b' && question.type != 'c');
    // �������ɱ��
    tiid(question.id, question.type);
    printf("\n    �������µ��������>>> ");
    scanf(" %[^\n]", question.txt);
    if (question.type == 'c') {
        question.chonum = 2;
        strcpy(question.choice[0], "��ȷ");
        strcpy(question.choice[1], "����");
    } else {
        do {
            printf("\n    �������µ�ѡ��������������20��>>> ");
            scanf("%d", &question.chonum);
            if (question.chonum <= 0 || question.chonum > 20) {
                printf("\n    ѡ����������������������룡����\n");
            }
        } while (question.chonum <= 0 || question.chonum > 20);

        for (int i = 0; i < question.chonum; i++) {
            printf("\n    ������� %d ����ѡ������>>> ", i + 1);
            scanf(" %[^\n]", question.choice[i]);
        }
    }
    int ansCount = 0;
    if (question.type == 'a') {
        do {
            printf("\n    �������µ���ȷ�𰸵�ѡ����ţ���1��ʼ��>>> ");
            scanf("%d", &question.ans[0]);
            if (question.ans[0] < 1 || question.ans[0] > question.chonum) {
                printf("\n    ���������������������룡����\n");
            }
        } while (question.ans[0] < 1 || question.ans[0] > question.chonum);
        ansCount = 1;
    } else if (question.type == 'b') {
        printf("\n    �������µ���ȷ�𰸵�ѡ����ţ��Կո�ָ������� -1 ������>>> ");
        int ans;
        ansCount = 0;
        while (1) {
            scanf("%d", &ans);
            if (ans == -1) break;
            if (ans < 1 || ans > question.chonum) {
                printf("\n    ���������������������룡����\n");
                continue;
            }
            question.ans[ansCount++] = ans;
        }
    } else {
        do {
            printf("\n    �������µ���ȷ�𰸵�ѡ����ţ�1: ��ȷ��2: ����>>> ");
            scanf("%d", &question.ans[0]);
            if (question.ans[0] < 1 || question.ans[0] > 2) {
                printf("\n    ���������������������룡����\n");
            }
        } while (question.ans[0] < 1 || question.ans[0] > 2);
        ansCount = 1;
    }

    fseek(fp, pos, SEEK_SET);
    fwrite(&question, sizeof(struct qqq), 1, fp);
    fclose(fp);
    printf("\n    ��Ŀ�޸ĳɹ�������������һ��");
    Sleep(1000);
    system("cls");
}
void tikulook()// ��Ŀ�鿴���� 
{
    SetColor(11);
    printf("\n|===================[--��---��---��---��--]\n");
    SetColor(15);

    FILE *fp = fopen(tkf, "rb");
    if (fp == NULL) {
        printf("\n    �ļ���ʧ�ܣ��鿴ʧ�ܣ�\n");
        return;
    }

    int mode;
    printf("\n    [1] �鿴��ѡ��\n");
    printf("\n    [2] �鿴��ѡ��\n");
    printf("\n    [3] �鿴�ж���\n");
    printf("\n    [4] �鿴ȫ������\n");
    printf("\n    [5] ����������鿴\n");
    printf("\n    ��ѡ��鿴ģʽ>>> ");
    scanf("%d", &mode);
    getchar();  
	system("cls"); 
    struct qqq question;
    int found = 0;
    switch (mode) {
        case 1:
            while (fread(&question, sizeof(struct qqq), 1, fp) == 1) {
                if (question.type == 'a') {
                    danxtprint(question);
                    printf("\n--------------------------------------------------------------");
                    found = 1;
                }
            }
            if (!found) {
                printf("\n    δ�ҵ���ѡ�⣡\n");
            }
            break;
        case 2:
            while (fread(&question, sizeof(struct qqq), 1, fp) == 1) {
                if (question.type == 'b') {
                    duoxtprint(question);
                    printf("\n--------------------------------------------------------------");
                    found = 1;
                }
            }
            if (!found) {
                printf("\n    δ�ҵ���ѡ�⣡\n");
            }
            break;
        case 3:
            while (fread(&question, sizeof(struct qqq), 1, fp) == 1) {
                if (question.type == 'c') {
                    pdtprint(question);
                    printf("\n--------------------------------------------------------------");
                    found = 1;
                }
            }
            if (!found) {
                printf("\n    δ�ҵ��ж��⣡\n");
            }
            break;
        case 4:
            while (fread(&question, sizeof(struct qqq), 1, fp) == 1) {
                switch (question.type) {
                    case 'a':
                        danxtprint(question);
                        break;
                    case 'b':
                        duoxtprint(question);
                        break;
                    case 'c':
                        pdtprint(question);
                        break;
                }
                printf("\n--------------------------------------------------------------");
            }
            break;
        case 5: {
            char id[5];
            printf("\n    ������Ҫ���ҵĿ�����>>> ");
            scanf("%s", id);
            getchar();  // ���ĵ� scanf �����Ļ��з�
            while (fread(&question, sizeof(struct qqq), 1, fp) == 1) {
                if (strcmp(question.id, id) == 0) {
                    switch (question.type) {
                        case 'a':
                            danxtprint(question);
                            break;
                        case 'b':
                            duoxtprint(question);
                            break;
                        case 'c':
                            pdtprint(question);
                            break;
                    }
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("\n    δ�ҵ����Ϊ %s �Ŀ��⣡\n", id);
            }
            break;
        }
        default:
            printf("\n    ��Ч��ѡ��\n");
    }
    fclose(fp);
    printf("\n    �������������һ��");
    getchar();
    system("cls");
}
void duoxtprint(struct qqq question) // ��ѡ��鿴��ӡ
{
    printf("\n��Ŀ��ţ�%s", question.id);
    printf("\n��Ŀ���ͣ���ѡ��\n");
    printf("\n��ɣ�%s", question.txt);
    for (int i = 0; i < question.chonum; i++) {
        printf("\n   %d.%s", i + 1, question.choice[i]);
    }
    printf("\n��ȷ�𰸣�");
    for (int i = 0; question.ans[i] != 0; i++) {
    	if(question.ans[i]>question.chonum||question.ans[i]<1) continue;
        printf(" %d ", question.ans[i]);
    }
}
void danxtprint(struct qqq question)// ��ѡ��鿴��ӡ
{
    printf("\n��Ŀ��ţ�%s", question.id);
    printf("\n��Ŀ���ͣ���ѡ��\n");
    printf("\n��ɣ�%s", question.txt);
    for (int i = 0; i < question.chonum; i++) {
        printf("\n   %d.%s", i + 1, question.choice[i]);
    }
    printf("\n\n��ȷ�𰸣� %d", question.ans[0]);
}
void pdtprint(struct qqq question) // �ж���鿴��ӡ
{
    printf("\n��Ŀ��ţ�%s", question.id);
    printf("\n��Ŀ���ͣ��ж���\n");
    printf("\n��ɣ�%s", question.txt);
    printf("\n   1.��ȷ");
    printf("\n   2.����");
    printf("\n��ȷ�𰸣� %d", question.ans[0]);
}
void teapwset(struct user *acc)//��ʦ�˸��������޸Ĺ��� 
{
	FILE *fp=fopen(teaaccf,"r+b");
	if (fp == NULL) // �ļ������ڣ���֤ʧ��
	{
		printf("�ļ������ڣ���֤ʧ��\n");
		exit(0);
	}
	struct user temp;
	while (fread(&temp, sizeof(struct user), 1, fp) == 1) if (strcmp(temp.acc, acc->acc) == 0 )
	{
    	break; // �˺�λ�ò�ѯ�ɹ� 	
	}
	fseek(fp,-sizeof(struct user),SEEK_CUR);//�ļ�ָ��Ǩ�����˺�λ��
	SetColor(11);
	printf("\n|===================[--��---��---��---��--]\n");
	SetColor(15);
	int ok=0;
	temp.pw[0]='\0';
	do{
		if(ok) printf("\n	[!] ��������������������룡\n");
		printf("\n	����֤����ԭ����>>> ");
		scanf("%[^\n]s",temp.pw);
		getchar();
		ok++;
	}while(strcmp(temp.pw, acc->pw) != 0);
	printf("\n	����������������>>> ");
	scanf("%[^\n]s",acc->pw);
	fwrite(acc,sizeof(struct user),1,fp);
	fclose(fp);
	printf("	\n�޸ĳɹ�!\n���س����ز˵���");
	scanf("%[^\n]s",&temp.acc);
	system("cls");
	getchar();
}
int stuCTRLGUI()//��ʦ��ѧ�����ܹ���ϵͳ��� ������0��ʾ�˳�ϵͳ 
{
	SetColor(11);
	printf("\n|===================[--ѧ---��---��---��---��---��--]\n");
	SetColor(15);
	printf("\n	[1]�˺����\n");
	printf("\n	[2]�˺�ɾ��\n");
	printf("\n	[3]�˺������������\n");
	printf("\n	[4]�˺�����ȫ������\n");
	printf("\n	[5]�鿴ѧ���˺ſ�\n") ;
	printf("\n	[0]������һ��\n");
	int cho,m=0;
	do{
	if(m>0) printf("\n[!] ���ƺ�������ˣ����������룡����\n");
		printf("\n�������Ĳ���>>> ");
		scanf("%d",&cho);
		getchar();
		m++; 
	}while(cho!=1&&cho!=2&&cho!=3&&cho!=0&&cho!=4&&cho!=5);
	system("cls");
	if(cho==0) return 0;
	else if(cho==1) stuCTRLadd();
	else if(cho==2) stuCTRLdel(); 
	else if(cho==3) stuCTRLset(1);
	else if(cho==4) stuCTRLset(0);
	else if(cho==5) stuCTRLlook(); 
	else pass();
	return 1;
}
void stuCTRLadd()//��ʦ��ѧ����Ϣ�������ѧ������ 
{
	SetColor(11);
	printf("\n|===================[--ѧ---��---��---��---��---��--]\n");
	SetColor(15);
	struct user acc;
	strcpy(acc.type,"ѧ��");
	int mark=0; 
	printf("\n	������ѧ������ >>> "),scanf("%s",acc.name);
	mark=0;
	do{
		if(mark>0) printf("\n	���˺��Ѵ��ڣ����޸ĺ��������룡����\n"); 
		printf("\n	�������봴��ѧ���ġ�Ψһ���˺�>>> "),scanf("%s",acc.acc);
		mark++;
	}while(acccfpd(acc.acc));
	printf("\n	���벢ȷ����������>>> "),scanf("%s",acc.pw);
// �����˺���Ϣ���ļ�
    FILE *fp = fopen(strcmp(acc.type,"��ʦ")==0 ? teaaccf : stuaccf, "ab");
    if (fp == NULL)
    {
        printf("\n	�ļ���ʧ�ܣ�ע��ʧ�ܣ�\n");
        return;
    }
    fwrite(&acc, sizeof(struct user), 1, fp); // ���û���Ϣд���ļ�
    fclose(fp);

    printf("\n	�����ɹ�������������һ��");
    Sleep(1000);
    system("cls");
}
void stuCTRLset(int mode) //ѧ���������ú��� 
{
	struct user temp;
	if(mode)
	{
		FILE *fp=fopen(stuaccf,"rb+");
		if (fp == NULL) // �ļ������ڣ���֤ʧ��
		{
			printf("�ļ������ڣ���֤ʧ��\n");
	        exit(0);
	    }
        SetColor(11);
        printf("\n|===================[--ѧ---��---��---��---��---��---��---��--]\n");
        SetColor(15);
        char act[30];
        int found,f=0;
        fseek(fp, 0, SEEK_SET);
        do{
            found=0;
            if(f) printf("[!] �˺Ų����ڣ����������룡");
            printf("\n��������Ҫ�����˺������ѧ�����˺ź�>>> ");
            scanf("%s", act);
            getchar();
            f++;
            while(fread(&temp,sizeof(struct user),1,fp)==1)
            {
                if(strcmp(temp.acc,act)==0)
                {
                    found=1;
                    break;
                }
            }
        }while(found==0);
        fseek(fp,-sizeof(struct user),SEEK_CUR);
        strcpy(temp.pw,mpw);
        fwrite(&temp,sizeof(struct user),1,fp); 
        fclose(fp);
    }
    else
    {
    // ����һ���㹻��Ľṹ���������洢����ѧ����Ϣ
    struct user students[maxusers];
    int studentCount = 0;

    // ���ļ���ȡ����ѧ����Ϣ
    FILE *fp = fopen(stuaccf, "rb");
    if (fp == NULL)
    {
        printf("�ļ���ʧ�ܣ��޷���ȡѧ����Ϣ\n");
        return;
    }

    // һ���Խ�����ѧ����Ϣ����ṹ������
    while (studentCount < maxusers && fread(&students[studentCount], sizeof(struct user), 1, fp) == 1)
    {
        studentCount++;
    }
    fclose(fp);

    // �޸�����ѧ��������ΪĬ������
    for (int i = 0; i < studentCount; i++)
    {
        strcpy(students[i].pw, mpw);
    }

    // ���ļ��Ը���д��ģʽ
    fp = fopen(stuaccf, "wb");
    if (fp == NULL)
    {
        printf("�ļ���ʧ�ܣ��޷�д��ѧ����Ϣ\n");
        return;
    }

    // ���޸ĺ��ѧ����Ϣд���ļ�
    for (int i = 0; i < studentCount; i++)
    {
        fwrite(&students[i], sizeof(struct user), 1, fp);
    }
    fclose(fp);
	}

    printf("\n    ����������Ϊ");
    SetColor(11);
    printf(mpw);
    SetColor(15);
    printf("\n    ����������һ���棡\n");
    Sleep(2000);
    system("cls");
}
void stuCTRLdel() // ��ʦ��ѧ����Ϣ����ɾ��ѧ������ 
{
    SetColor(11);
    printf("\n|===================[--ѧ---��---��---��---ɾ---��--]\n");
    SetColor(15);
    struct user students[maxusers];
    int studentCount = 0;
    // ���ļ���ȡ����ѧ����Ϣ
    FILE *fp = fopen(stuaccf, "rb");
    if (fp == NULL) {
        printf("�ļ���ʧ�ܣ��޷���ȡѧ����Ϣ\n");
        return;
    }
    while (studentCount < maxusers && fread(&students[studentCount], sizeof(struct user), 1, fp) == 1) // һ���Խ�����ѧ����Ϣ����ṹ������
	{
        studentCount++;
    }
    fclose(fp);
    char act[30];
    int found = 0;
    int targetIndex = -1;
    int f = 0;
    do {
        found = 0;
        if (f) printf("[!] �˺Ų����ڣ����������룡");
        printf("\n��������Ҫɾ����ѧ�����˺ź�>>> ");
        scanf("%s", act);
        getchar();
        f++;
        for (int i = 0; i < studentCount; i++) {
            if (strcmp(students[i].acc, act) == 0) {
                found = 1;
                targetIndex = i;
                break;
            }
        }
    }while (found == 0);
    for (int i = targetIndex; i < studentCount - 1; i++)
	{
        students[i] = students[i + 1];
    }
    studentCount--;
    fp = fopen(stuaccf, "wb");
    if (fp == NULL) {
        printf("�ļ���ʧ�ܣ��޷�д��ѧ����Ϣ\n");
        return;
    }
    for (int i = 0; i < studentCount; i++) {
        fwrite(&students[i], sizeof(struct user), 1, fp);
    }
    fclose(fp);
    printf("\n    �˺�ɾ���ɹ���\n    ����������һ���棡\n");
    Sleep(2000);
    system("cls");
}
void stuCTRLlook()//�鿴ѧ���˺ſ�
{
	FILE *fp=fopen(stuaccf,"rb");
	if (fp == NULL) {
        printf("�ļ���ʧ�ܣ��޷���ȡѧ����Ϣ\n");
        return;
    }
	struct user temp;
	while(fread(&temp,sizeof(struct user),1,fp)==1)
	{
		printf("�˺ţ�%s\n",temp.acc);
		printf("���룺%s\n",temp.pw);
		printf("������%s\n",temp.name);
		printf("-------------------------------------------------------------------\n");
	} 
	fclose(fp);
	printf("\n�����������>>> ");
	scanf("[^\n]",temp.acc);
	system("cls");
} 
void tealook()//��ʦ�˲�ѯѧ������������ 
{
	SetColor(11);
	printf("\n|===================[--ѧ---��---��---��---��---ѯ--]\n");
	SetColor(15); 
	printf("\n	[1]�鿴ָ��ѧ�������¼\n");
	printf("\n	[2]�鿴ȫ�������¼\n");
	printf("\n	[0]����");
	int cho,m=0;
	do{
		if(m>0) printf("\n[!] ���ƺ�������ˣ����������룡����\n");
		printf("\n�������Ĳ���>>> ");
		scanf("%d",&cho);
		getchar();
		m++; 
	}while(cho!=1&&cho!=2&&cho!=0);
	system("cls");
	if(cho==0) return;
	else if(cho==1) relookalone();
	else if(cho==2) relookall();
}
void relookalone()//��ѯ������
{
	printf("\n	������ѧ������>>> ");
	char ac[30];
	scanf("%[^\n]",ac);
	getchar();
	FILE* fp=fopen(resf,"rb");
	struct res temp;
	while(fread(&temp,sizeof(struct res),1,fp)==1) if(strcmp(ac,temp.name)==0) 
	{
		printf("\n============================================================\n");
		printf("������%s\n",temp.name);
		printf("������%d/%d\n",temp.sco,temp.man);
		printf("���������\n");
		for(int i=0;i<temp.tinum;i++)
		{
			printf("	%s:",temp.tilist[i]);
			if(temp.torf[i]==1) printf("��ȷ\n");
			else printf("����\n"); 
		}
		printf("��������:%s",temp.date);
	}
	fclose(fp);
	printf("\n�����������>>> ");
	scanf("[^\n]",ac);
	system("cls");
}
void relookall()//��ѯ���� 
{
	FILE* fp=fopen(resf,"rb");
	struct res temp;
	while(fread(&temp,sizeof(struct res),1,fp)==1) 
	{
		printf("\n============================================================\n") ;
		printf("������%s\n",temp.name);
		printf("������%d/%d\n",temp.sco,temp.man);
		printf("���������\n");
		for(int i=0;i<temp.tinum;i++)
		{
			printf("	%s:",temp.tilist[i]);
			if(temp.torf[i]==1) printf("��ȷ\n");
			else printf("����\n"); 
		}
		printf("��������:%s",temp.date);
	}
	fclose(fp);
	printf("\n�����������>>> ");
	scanf("[^\n]",temp.name);
	system("cls");
}
