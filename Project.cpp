#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
//定义题目种类分值，单选，多选 ,判断，最大用户数 
#define scodan 3 
#define scoduo 5 
#define scopd 2 
#define maxusers 100
//定义数据保存文件,依次是教师账号，学生账号，题库，答题结果 ,默认学生密码，考卷最大考题数 
#define teaaccf "teacher.dat" 
#define stuaccf "student.dat" 
#define tkf "tm.dat"
#define resf "result.dat"
#define mpw "123"
#define qmax 50
// 定义默认在线测试题目数量
int Sq = 3;
int Mq = 3;
int Jq = 3;
struct user{//账号结构，保存教师学生姓名，账号与密码 
	char name[30];
	char acc[30];
	char pw[30];
	char type[8];
}; 
struct qqq{//考题结构，保存题题干，答案与选项 ，题目编号 
	char txt[2000];
	char choice[20][100];
	int chonum;
	int ans[20];//答案 
	char id[5];//唯一题目编号 
	char type;//a代表单选，b代表多选，c代表判断题 
};
struct res{//答题记录结构，保存在线答题记录 
	char name[30];
	char tilist[50][6];//题目编号列表 
	int tinum;//题目数 
	int torf[50];//正确与否 
	int sco;//分数
	int man;//满分分数 
	char date[20];//日期 
}; 
int GUI();//主GUI入口 
void signACC();//教师与学生账号注册函数 
	int acccfpd(const char* chs);//账号重复判断，重复返回1，不重复返回0 
int dengluGUI();//登陆界面GUI以及学生和教师GUI函数入口 ，返回0表示退出系统 
	int accyzpd(struct user *acc); //验证账号，错误返回1，正确返回0
	int stuGUI(struct user *acc);//学生界面GUI以及学生功能函数入口 ，返回0表示退出系统 
		void stupwset(struct user *acc);//学生端个人密码修改功能 
		void exam(struct user* acc);//学生端在线答题功能
			void tkzuhe(struct qqq* examQuestions);//答题题库随机组合函数
			void dlcho(struct qqq* qq);//打乱选项顺序 
			void update(char name[], struct qqq* Ex, int* torf, int score, int man);//学生答题记录更新函数 
		void stulook(struct user* acc);//查询学生个人答题情况
	int teaGUI(struct user *acc);//教师界面GUI以及学生功能函数入口，返回0表示退出系统 
		int tikuGUI();//教师端题库操作GUI函数 
			void tikuadd();//题库扩充 
				void tiid(char *id, char questionType);//生成新题目编号
			void tikudel();//题目删除 
			void tikuset();//题目修改
			void tikulook();//题库查看
				void duoxtprint(struct qqq question);//多选题打印
				void danxtprint(struct qqq question);//单选题打印
				void pdtprint(struct qqq question);//判断题打印
		void teapwset(struct user *acc);//教师端个人密码修改功能 
		int stuCTRLGUI();//教师端学生账密管理系统入口 ，返回0表示退出系统 
			void stuCTRLadd();//教师端学生信息管理添加学生函数 
			void stuCTRLset(int mode);//教师端学生信息管理修改学生信息函数
			void stuCTRLdel();//教师端学生信息管理删除学生函数
			void stuCTRLlook();//学生账号库查看 
		void tealook();//教师端查询学生答题情况入口
			void relookalone();//查询单个人
			void relookall();//查询所有
void tip()//程序注释 
{
	printf("\n				       【程序注释】\n\n");
	printf("\n	*在本程序账号系统中，账号号有唯一性，其他属性，姓名，密码均可重复\n");
	printf("\n	*本程序第一次使用时需先注册教师和学生类型的两个账号\n");
	printf("\n	*为方便管理，本程序的题库中每一道题都有其专属编号，专属编号在新增\n");
	printf("\n  题目时自动生成，格式为“（字母）+（带有前导0的三位数字）”，其中\n");
	printf("\n  字母代表题目类型，A代表单选题，B代表多选题，C代表判断题；数字代\n");
	printf("\n  表它是该类型题目的第几道题，如B003是指第三个加入题库中多选题,\n");
	printf("\n  删除题目时，其他题目编号不变，但修改题目时，编号将会更新未当前类型最大题目编号+1。\n");
	printf("\n	*当前学生账号重置默认密码为 %s\n",mpw);
	printf("\n	*题库系统中一道题最多支持20个选项，生成考卷时，将从题库随机抽取\n\n  固定数量考题，目前默认考卷组合为 %d道单选题+%d道多选题+%d道判断题\n",Sq,Mq,Jq);
	printf("\n	*当前题目分值为单选题%d分，多选题%d分，判断题%d分\n",scodan,scoduo,scopd); 
	printf("\n  且单选题和多选题的选项顺序会被随机打乱\n"); 
	printf("\n	*待补充......\n");
	char s=getchar();
	system("cls");
}
void SetColor(int ForgC) //UI外观颜色设置函数 
{
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}
int szpd(int a[], int b[]) // 辅助函数，比较a[]和b[]是否是含有相同元素的数组，用于考试模块计算得分
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
void shufflePartialArray(void* array, int size, int elemSize) // 随机打乱结构体数组函数
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
void pass()//待完善提示模块，用于程序开发维护使用 
{
	SetColor(8);
	printf("*该功能待完善");
	SetColor(15);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	SetConsoleTitle("思政知识学习测试系统（巅峰版）");//窗口标题 
	SetColor(15);
	while(GUI()!=0);
	return 0;
}
//***************************************************************************************************************************
int GUI() //主GUI入口
{
	for(int i=0;i<65;i++)
	{
//		Sleep(100);
        printf("_");
    }
    printf("\n");
	printf("                                                               ||\n");
	SetColor(12);
    printf("            THE 思政知识学习测试系统（巅峰版）                 ");
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
    printf("\n请选择您的操作:\n\n");
    Sleep(100);
    printf("    [1]登陆\n\n");
    Sleep(100);
    printf("    [2]注册\n\n");
    Sleep(100);
    printf("    [3]关于\n\n");
    Sleep(100);
    printf("    [0]关闭程序\n\n ");
    Sleep(100);
	SetColor(11);
    printf("请输入相应数字");
    for (int i = 0; i < 3; i++)
	{
        Sleep(10);
        printf(">");
    }
    SetColor(15);
    int m=0;
	do{
		SetColor(11);
		if(m>0) printf("\n\a[!] 未知选择，请重新输入>>> ");
		SetColor(15);
		scanf("%[^\n]", &n);
		getchar();
		m++;
	}while(n!='0'&&n!='1'&&n!='2'&&n!='3');
	system("cls");//清空文本 
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
void signACC() //教师与学生账号注册函数
{
	struct user acc;
	SetColor(11);
	printf("\n|===================[--账---号---注---册--]\n");
	SetColor(15);
	int mark=0; 
	do{
		if(mark>0) printf("\n\a	输入错误，请重新输入！！！\n");
		printf("\n	请输入账号身份（老师/学生）>>> "),scanf("%s",acc.type);
		mark++;
	}while(strcmp(acc.type,"老师")!=0&&strcmp(acc.type,"学生")!=0);
	printf("\n	请输入您的称呼 >>> "),scanf("%s",acc.name);
	printf("\n	好的，尊敬的%s\n",acc.name);
	mark=0;
	do{
		if(mark>0) printf("\n	该账号已存在，请修改后重新输入！！！\n"); 
		printf("\n	接下来请创建您的【唯一】账号>>> "),scanf("%s",acc.acc);
		mark++;
	}while(acccfpd(acc.acc));
	printf("\n	输入并确认您的密码>>> "),scanf("%s",acc.pw);
	getchar();
// 保存账号信息到文件
    FILE *fp = fopen(strcmp(acc.type,"老师")==0 ? teaaccf : stuaccf, "ab");
    if (fp == NULL)
    {
        printf("\n	文件打开失败，注册失败！\n");
        return;
    }
    fwrite(&acc, sizeof(struct user), 1, fp); // 将用户信息写入文件
    fclose(fp); 
    printf("\n	注册成功！即将返回上一级");
    Sleep(1000);
    system("cls");
}
int acccfpd(const char* chs)
{
	FILE* tfp = fopen(teaaccf, "rb");
	FILE* sfp = fopen(stuaccf, "rb");
	if (tfp == NULL|| sfp == NULL) return 0; // 如果文件不存在，说明没有重复
	
	struct user temp;
	while (fread(&temp, sizeof(struct user), 1, tfp) == 1)
	{
	    if (strcmp(temp.acc, chs) == 0)
	    {
	            fclose(tfp);
	            return 1; // 找到重复账号
	    }
    }
    while (fread(&temp, sizeof(struct user), 1, sfp) == 1)
	{
	    if (strcmp(temp.acc, chs) == 0)
	    {
	        fclose(sfp);
	        return 1; // 找到重复账号
	    }
    }
    fclose(tfp);
    fclose(sfp);
    return 0; // 没有找到重复账号
}
int dengluGUI()//登陆界面GUI以及学生和教师GUI函数入口 ，返回0表示退出系统 
{
	struct user acc;
	SetColor(11);
	printf("\n|===================[--账---号---登---录--]\n");
	SetColor(15);
	int mark=0;
	do{
		if(mark>0) printf("\n\a[!] 账号或密码错误，请重新输入！！！\n");
		printf("\n	请输入您的账号>>> "),scanf("%s",acc.acc);
		getchar();
		printf("\n	请输入您的密码>>> "),scanf("%s",acc.pw);
		getchar();
		mark++;
	}while(accyzpd(&acc));
	system("cls");
	if(strcmp(acc.type,"老师") ==0) while(teaGUI(&acc));
	else while(stuGUI(&acc));
	return 1;
}
int accyzpd(struct user *acc)//验证账号，错误返回1，正确返回0,同时从账户库中补全信息 
{
	FILE* tfp = fopen(teaaccf, "rb");
	FILE* sfp = fopen(stuaccf, "rb");
    if (tfp == NULL||sfp==NULL) // 文件不存在，验证失败
	{
		printf("文件不存在，验证失败\n");
		return 1;
	}
    struct user temp;
    while (fread(&temp, sizeof(struct user), 1, tfp) == 1)
    {
        if (strcmp(temp.acc, acc->acc) == 0 && strcmp(temp.pw, acc->pw) == 0)
        {
            *acc = temp; // 补全信息
            fclose(tfp);
            fclose(sfp);
            return 0; // 验证成功
        }
    }
    while (fread(&temp, sizeof(struct user), 1, sfp) == 1)
    {
        if (strcmp(temp.acc, acc->acc) == 0 && strcmp(temp.pw, acc->pw) == 0)
        {
            *acc = temp; // 补全信息
            fclose(tfp);
            fclose(sfp);
            return 0; // 验证成功
        }
    }
    fclose(sfp);
    fclose(tfp);
    return 1; // 验证失败 	
} 
int stuGUI(struct user *acc)//学生界面GUI以及学生功能函数入口 ，返回0表示退出系统 
{
	SetColor(11);
	printf("\n\nQAQ/ 欢迎 %s 同学\n",acc->name);
	SetColor(15);
	printf("\n	[1]在线测试\n");
	printf("\n	[2]修改个人密码\n");
	printf("\n	[3]个人答题情况查询\n");
	printf("\n	[0]退出登录\n");
	int cho,m=0;
	do{
		if(m>0) printf("\n[!] 您似乎按错键了，请重新输入！！！\n");
		printf("\n输入您的操作>>> ");
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
void stupwset(struct user *acc)//学生端个人密码修改功能 
{
	FILE *fp=fopen(stuaccf,"r+b");
	if (fp == NULL) // 文件不存在，验证失败
	{
		printf("文件不存在，验证失败\n");
		exit(0);
	}
	struct user temp;
	while (fread(&temp, sizeof(struct user), 1, fp) == 1) if (strcmp(temp.acc, acc->acc) == 0 )
	{
    	break; // 账号位置查询成功 	
	}
	fseek(fp,-sizeof(struct user),SEEK_CUR);//文件指针迁移至账号位置
	SetColor(11);
	printf("\n|===================[--密---码---修---改--]\n");
	SetColor(15);
	int ok=0;
	temp.pw[0]='\0';
	do{
		if(ok) printf("\n	[!] 密码输入错误，请重新输入！\n");
		printf("\n	请验证您的原密码>>> ");
		scanf("%[^\n]s",temp.pw);
		getchar();
		ok++;
	}while(strcmp(temp.pw, acc->pw) != 0);
	printf("\n	请输入您的新密码>>> ");
	scanf("%[^\n]s",acc->pw);
	fwrite(acc,sizeof(struct user),1,fp);
	fclose(fp);
	printf("	\n修改成功!\n按回车返回菜单！");
	scanf("%[^\n]",&temp.acc);
	system("cls");
	getchar();
}
void exam(struct user* acc) //答题函数 
{
	int ma=0,zdyex;
	printf("\n是否使用自定义考题数量？\n([1] 使用,[0]不使用(使用默认考题数) )>>> ");
	scanf("%d",&zdyex);
	if(zdyex==1)
	{
		printf("请依次输入单选题，多选题，判断题数量(总题数最多不超过%d)>>> ",qmax);
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
//        for(int i=0;currentQuestion.ans[i]!=0&&i<20;i++) printf(" %d",currentQuestion.ans[i]);    打印答案，测试代码用 
		printf("\n");
        if (currentQuestion.type == 'a' || currentQuestion.type == 'b') 
		{
            for (int i = 0; i < currentQuestion.chonum; i++) {
                printf("%d. %s\n", i + 1, currentQuestion.choice[i]);
            }
            printf("输入答案（多选用空格分隔)：");
            int answer;
            int validInput = 0;
            int inputCount = 0;
            char input[100];
            scanf(" %[^\n]", input); // 正确读取多选题答案
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
            printf("1. 正确\n2. 错误\n");
            printf("输入答案（1或2）：");
            int answer;
            while (1) {
                scanf("%d", &answer);
                if (answer == 1 || answer == 2)
				{
                    break;
                }
                printf("输入错误，请重新输入：");
            }
            stuans[ti][0] = answer;
        }
        // 判断答案是否正确
        int correct = 0;
        if (currentQuestion.type == 'a') correct = (stuans[ti][0] == currentQuestion.ans[0]);
		else if (currentQuestion.type == 'b') correct = szpd(currentQuestion.ans, stuans[ti]);
		else if (currentQuestion.type == 'c') correct = (stuans[ti][0] == currentQuestion.ans[0]);
        torf[ti] = correct;
        if (correct) sco += (currentQuestion.type == 'a' ? scodan : (currentQuestion.type == 'b' ? scoduo : scopd));
        ti++;
        if (ti < Exl)
		{
            system("cls"); // 确保清屏
        }
    }
    printf("考试结束！你的得分是 %d/%d\n\n可前往个人答题记录查看详情", sco, man);
    update(acc->name, Ex, torf, sco, man);
    Sleep(2000);
    system("cls");
}
void tkzuhe(struct qqq* examQuestions) //随机抽取考题生成试卷
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
        printf("\n题库题目不足，考卷生成失败！\n");
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
void dlcho(struct qqq *qq) // 打乱选项顺序函数，并正确跟踪选项位置变化
{
    if (qq->type == 'c') return;
    int n = qq->chonum;
    int old_to_new[20];   // 旧索引到新索引的映射
    // 初始化 old_to_new 数组为原始顺序
    for (int i = 0; i < n; i++) 
	{
        old_to_new[i] = i;
    }
    // 使用Knuth洗牌算法打乱选项
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);  // 随机选择要交换的元素

        // 交换选项
        char temp[100];
        strcpy(temp, qq->choice[i]);
        strcpy(qq->choice[i], qq->choice[j]);
        strcpy(qq->choice[j], temp);

        // 更新 old_to_new 数组
        // 交换 old_to_new[i] 和 old_to_new[j]
        int swap = old_to_new[i];
        old_to_new[i] = old_to_new[j];
        old_to_new[j] = swap;
    }

    // 调整答案数组中的索引
    for (int i = 0; qq->ans[i] != 0; i++) {
        int old_index = qq->ans[i]; // 原答案中的选项索引（1-based）
        if (old_index >= 1 && old_index <= n) { // 原索引有效
            // 找到 old_to_new 数组中对应的旧索引
            int new_idx = -1;
            for (int k = 0; k < n; k++) {
                if (old_to_new[k] == old_index - 1) {  // 原索引转换为 0-based
                    new_idx = k;
                    break;
                }
            }
            if (new_idx != -1)
			{
                qq->ans[i] = new_idx + 1;  // 更新为新索引（1-based）
            }
        }
    }
}
void update(char name[], struct qqq* Ex, int* torf, int score, int man) //保存答题记录函数 
{
    struct res result;
    strcpy(result.name, name);
    result.sco = score;
    result.man = man;
    time_t t = time(NULL);//获取当前时间戳 
    struct tm* tm = localtime(&t);//时间戳转化 
    strftime(result.date, sizeof(result.date), "%Y-%m-%d-%H:%M", tm);//格式化保存时间戳 
    result.tinum = Sq + Mq + Jq;
    for (int i = 0; i < result.tinum; i++) {
        strcpy(result.tilist[i], Ex[i].id);
        result.torf[i] = torf[i];
    }

    FILE* fp = fopen(resf, "ab");
    if (fp == NULL) {
        printf("文件打开失败，无法保存答题记录！\n");
        return;
    }
    fwrite(&result, sizeof(struct res), 1, fp);
    fclose(fp);
}
void stulook(struct user* acc) //学生答题记录查询函数 
{
    FILE* fp = fopen(resf, "rb");
    if (fp == NULL) {
        printf("结果文件打开失败！\n");
        return;
    }

    struct res temp;
    char name[30];
    strcpy(name, acc->name); // 自动使用当前学生姓名

    while (fread(&temp, sizeof(struct res), 1, fp) == 1) {
        if (strcmp(temp.name, name) == 0) {
            printf("============================================================\n");
            printf("姓名：%s\n", temp.name);
            printf("分数：%d/%d\n", temp.sco, temp.man);
            printf("答题情况：\n");
            for (int i = 0; i < temp.tinum; i++) {
                printf("题目编号：%s，答题情况：%s\n", temp.tilist[i], temp.torf[i] ? "正确" : "错误");
            }
            printf("答题日期：%s\n",temp.date);
        }
    }

    printf("\n按任意键返回>>> ");
    getchar();
    system("cls");
    fclose(fp);
}
int teaGUI(struct user *acc)//教师界面GUI以及学生功能函数入口，返回0表示退出系统 
{
	SetColor(11);
	printf("\n\n0w0/ 欢迎 %s 老师\n",acc->name);
	SetColor(15);
	printf("\n	[1]题库管理\n");
	printf("\n	[2]修改个人密码\n");
	printf("\n	[3]管理学生账号\n");
	printf("\n	[4]学生答题情况查询\n");
	printf("\n	[0]退出登录\n");
	int cho,m=0;
	do{
		if(m>0) printf("\n[!] 您似乎按错键了，请重新输入！！！\n");
		printf("\n输入您的操作>>> ");
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
int tikuGUI()//教师端题库操作GUI函数 
{
	SetColor(11);
	printf("\n|===================[--题---库---管---理--]\n");
	SetColor(15);
	printf("\n	[1]新增考题\n");
	printf("\n	[2]考题修改\n");
	printf("\n	[3]考题删除\n");
	printf("\n	[4]考题查看\n");
	printf("\n	[0]返回\n");
	int cho,m=0;
	do{
	if(m>0) printf("\n[!] 您似乎按错键了，请重新输入！！！\n");
	printf("\n输入您的操作>>> ");
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
void tikuadd() // 教师端题库扩充函数
{
    SetColor(11);
    printf("\n|===================[--题---库---扩---充--]\n");
    SetColor(15);

    struct qqq question;
	for(int i=0;i<20;i++) question.ans[i]=0;
    int mark = 0;
    do {
        if (mark > 0) printf("\n    输入错误，请重新输入！！！\n");
        printf("\n    请输入题目类型（a: 单选，b: 多选，c: 判断）>>> ");
        scanf(" %c", &question.type);
        mark++;
    } while (question.type != 'a' && question.type != 'b' && question.type != 'c');

    // 生成新题目编号
    tiid(question.id, question.type);

    printf("\n    请输入题干内容>>> ");
    scanf(" %[^\n]", question.txt);

    if (question.type == 'c') {
        question.chonum = 2;
        strcpy(question.choice[0], "正确");
        strcpy(question.choice[1], "错误");
    } else {
        do {
            printf("\n    请输入选项数量（不超过20）>>> ");
            scanf("%d", &question.chonum);
            if (question.chonum <= 0 || question.chonum > 20) {
                printf("\n    选项数量输入错误，请重新输入！！！\n");
            }
        } while (question.chonum <= 0 || question.chonum > 20);

        for (int i = 0; i < question.chonum; i++) {
            printf("\n    请输入第 %d 个选项内容>>> ", i + 1);
            scanf(" %[^\n]", question.choice[i]);
        }
    }

    int ansCount = 0;
    if (question.type == 'a') {
        do {
            printf("\n    请输入正确答案的选项序号（从1开始）>>> ");
            scanf("%d", &question.ans[0]);
            if (question.ans[0] < 1 || question.ans[0] > question.chonum) {
                printf("\n    答案序号输入错误，请重新输入！！！\n");
            }
        } while (question.ans[0] < 1 || question.ans[0] > question.chonum);
        ansCount = 1;
    } else if (question.type == 'b') {
        printf("\n    请输入正确答案的选项序号（以空格分隔，输入 -1 结束）>>> ");
        int ans;
        ansCount = 0;
        while (1) {
            scanf("%d", &ans);
            if (ans == -1) break;
            if (ans < 1 || ans > question.chonum) {
                printf("\n    答案序号输入错误，请重新输入！！！\n");
                continue;
            }
            question.ans[ansCount++] = ans;
        }
    } else {
        do {
            printf("\n    请输入正确答案的选项序号（1: 正确，2: 错误）>>> ");
            scanf("%d", &question.ans[0]);
            if (question.ans[0] < 1 || question.ans[0] > 2) {
                printf("\n    答案序号输入错误，请重新输入！！！\n");
            }
        } while (question.ans[0] < 1 || question.ans[0] > 2);
        ansCount = 1;
    }

    // 保存题目信息到文件
    FILE *fp = fopen(tkf, "ab");
    if (fp == NULL) {
        printf("\n    文件打开失败，题目添加失败！\n");
        return;
    }
    fwrite(&question, sizeof(struct qqq), 1, fp);
    fclose(fp);

    printf("\n    题目添加成功！即将返回上一级");
    Sleep(1000);
    system("cls");
}
void tiid(char *id, char qType) // 生成新题目编号
{
    FILE *fp = fopen(tkf, "rb");
    if (fp == NULL) {
        // 如果文件不存在，说明是第一道题
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
void tikudel() // 题目删除函数
{
    SetColor(11);
    printf("\n|===================[--题---目---删---除--]\n");
    SetColor(15);

    char id[5];
    printf("\n    请输入要删除的题目编号>>> ");
    scanf("%s", id);

    FILE *fp = fopen(tkf, "rb");
    if (fp == NULL) {
        printf("\n    文件打开失败，删除失败！\n");
        return;
    }

    struct qqq questions[maxusers];
    int count = 0;
    int found = 0;
    while(fread(&questions[count], sizeof(struct qqq), 1, fp) == 1)//查找并左移数组 
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
        printf("\n    未找到该题目编号，删除失败！\n");
        Sleep(1000);
        system("cls");
        return;
    }

    fp = fopen(tkf, "wb");
    if (fp == NULL) {
        printf("\n    文件打开失败，删除失败！\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fwrite(&questions[i], sizeof(struct qqq), 1, fp);
    }
    fclose(fp);

    printf("\n    题目删除成功！即将返回上一级");
    Sleep(1000);
    system("cls");
}
void tikuset() // 题目修改函数
{
    SetColor(11);
    printf("\n|===================[--题---目---修---改--]\n");
    SetColor(15);

    char id[5];
    printf("\n    请输入要修改的题目编号>>> ");
    scanf("%s", id);

    FILE *fp = fopen(tkf, "rb+");
    if (fp == NULL) {
        printf("\n    文件打开失败，修改失败！\n");
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
        printf("\n    未找到该题目编号，修改失败！\n");
        fclose(fp);
        Sleep(1000);
        system("cls");
        return;
    }

    int mark = 0;
    do {
        if (mark > 0) printf("\n    输入错误，请重新输入！！！\n");
        printf("\n    请输入新的题目类型（a: 单选，b: 多选，c: 判断）>>> ");
        scanf(" %c", &question.type);
        mark++;
    } while (question.type != 'a' && question.type != 'b' && question.type != 'c');
    // 重新生成编号
    tiid(question.id, question.type);
    printf("\n    请输入新的题干内容>>> ");
    scanf(" %[^\n]", question.txt);
    if (question.type == 'c') {
        question.chonum = 2;
        strcpy(question.choice[0], "正确");
        strcpy(question.choice[1], "错误");
    } else {
        do {
            printf("\n    请输入新的选项数量（不超过20）>>> ");
            scanf("%d", &question.chonum);
            if (question.chonum <= 0 || question.chonum > 20) {
                printf("\n    选项数量输入错误，请重新输入！！！\n");
            }
        } while (question.chonum <= 0 || question.chonum > 20);

        for (int i = 0; i < question.chonum; i++) {
            printf("\n    请输入第 %d 个新选项内容>>> ", i + 1);
            scanf(" %[^\n]", question.choice[i]);
        }
    }
    int ansCount = 0;
    if (question.type == 'a') {
        do {
            printf("\n    请输入新的正确答案的选项序号（从1开始）>>> ");
            scanf("%d", &question.ans[0]);
            if (question.ans[0] < 1 || question.ans[0] > question.chonum) {
                printf("\n    答案序号输入错误，请重新输入！！！\n");
            }
        } while (question.ans[0] < 1 || question.ans[0] > question.chonum);
        ansCount = 1;
    } else if (question.type == 'b') {
        printf("\n    请输入新的正确答案的选项序号（以空格分隔，输入 -1 结束）>>> ");
        int ans;
        ansCount = 0;
        while (1) {
            scanf("%d", &ans);
            if (ans == -1) break;
            if (ans < 1 || ans > question.chonum) {
                printf("\n    答案序号输入错误，请重新输入！！！\n");
                continue;
            }
            question.ans[ansCount++] = ans;
        }
    } else {
        do {
            printf("\n    请输入新的正确答案的选项序号（1: 正确，2: 错误）>>> ");
            scanf("%d", &question.ans[0]);
            if (question.ans[0] < 1 || question.ans[0] > 2) {
                printf("\n    答案序号输入错误，请重新输入！！！\n");
            }
        } while (question.ans[0] < 1 || question.ans[0] > 2);
        ansCount = 1;
    }

    fseek(fp, pos, SEEK_SET);
    fwrite(&question, sizeof(struct qqq), 1, fp);
    fclose(fp);
    printf("\n    题目修改成功！即将返回上一级");
    Sleep(1000);
    system("cls");
}
void tikulook()// 题目查看函数 
{
    SetColor(11);
    printf("\n|===================[--题---库---查---看--]\n");
    SetColor(15);

    FILE *fp = fopen(tkf, "rb");
    if (fp == NULL) {
        printf("\n    文件打开失败，查看失败！\n");
        return;
    }

    int mode;
    printf("\n    [1] 查看单选题\n");
    printf("\n    [2] 查看多选题\n");
    printf("\n    [3] 查看判断题\n");
    printf("\n    [4] 查看全部考题\n");
    printf("\n    [5] 考题编号搜题查看\n");
    printf("\n    请选择查看模式>>> ");
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
                printf("\n    未找到单选题！\n");
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
                printf("\n    未找到多选题！\n");
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
                printf("\n    未找到判断题！\n");
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
            printf("\n    请输入要查找的考题编号>>> ");
            scanf("%s", id);
            getchar();  // 消耗掉 scanf 遗留的换行符
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
                printf("\n    未找到编号为 %s 的考题！\n", id);
            }
            break;
        }
        default:
            printf("\n    无效的选择！\n");
    }
    fclose(fp);
    printf("\n    按任意键返回上一级");
    getchar();
    system("cls");
}
void duoxtprint(struct qqq question) // 多选题查看打印
{
    printf("\n题目编号：%s", question.id);
    printf("\n题目类型：多选题\n");
    printf("\n题干：%s", question.txt);
    for (int i = 0; i < question.chonum; i++) {
        printf("\n   %d.%s", i + 1, question.choice[i]);
    }
    printf("\n正确答案：");
    for (int i = 0; question.ans[i] != 0; i++) {
    	if(question.ans[i]>question.chonum||question.ans[i]<1) continue;
        printf(" %d ", question.ans[i]);
    }
}
void danxtprint(struct qqq question)// 单选题查看打印
{
    printf("\n题目编号：%s", question.id);
    printf("\n题目类型：单选题\n");
    printf("\n题干：%s", question.txt);
    for (int i = 0; i < question.chonum; i++) {
        printf("\n   %d.%s", i + 1, question.choice[i]);
    }
    printf("\n\n正确答案： %d", question.ans[0]);
}
void pdtprint(struct qqq question) // 判断题查看打印
{
    printf("\n题目编号：%s", question.id);
    printf("\n题目类型：判断题\n");
    printf("\n题干：%s", question.txt);
    printf("\n   1.正确");
    printf("\n   2.错误");
    printf("\n正确答案： %d", question.ans[0]);
}
void teapwset(struct user *acc)//教师端个人密码修改功能 
{
	FILE *fp=fopen(teaaccf,"r+b");
	if (fp == NULL) // 文件不存在，验证失败
	{
		printf("文件不存在，验证失败\n");
		exit(0);
	}
	struct user temp;
	while (fread(&temp, sizeof(struct user), 1, fp) == 1) if (strcmp(temp.acc, acc->acc) == 0 )
	{
    	break; // 账号位置查询成功 	
	}
	fseek(fp,-sizeof(struct user),SEEK_CUR);//文件指针迁移至账号位置
	SetColor(11);
	printf("\n|===================[--密---码---修---改--]\n");
	SetColor(15);
	int ok=0;
	temp.pw[0]='\0';
	do{
		if(ok) printf("\n	[!] 密码输入错误，请重新输入！\n");
		printf("\n	请验证您的原密码>>> ");
		scanf("%[^\n]s",temp.pw);
		getchar();
		ok++;
	}while(strcmp(temp.pw, acc->pw) != 0);
	printf("\n	请输入您的新密码>>> ");
	scanf("%[^\n]s",acc->pw);
	fwrite(acc,sizeof(struct user),1,fp);
	fclose(fp);
	printf("	\n修改成功!\n按回车返回菜单！");
	scanf("%[^\n]s",&temp.acc);
	system("cls");
	getchar();
}
int stuCTRLGUI()//教师端学生账密管理系统入口 ，返回0表示退出系统 
{
	SetColor(11);
	printf("\n|===================[--学---生---账---号---管---理--]\n");
	SetColor(15);
	printf("\n	[1]账号添加\n");
	printf("\n	[2]账号删除\n");
	printf("\n	[3]账号密码个人重置\n");
	printf("\n	[4]账号密码全体重置\n");
	printf("\n	[5]查看学生账号库\n") ;
	printf("\n	[0]返回上一级\n");
	int cho,m=0;
	do{
	if(m>0) printf("\n[!] 您似乎按错键了，请重新输入！！！\n");
		printf("\n输入您的操作>>> ");
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
void stuCTRLadd()//教师端学生信息管理添加学生函数 
{
	SetColor(11);
	printf("\n|===================[--学---生---账---号---添---加--]\n");
	SetColor(15);
	struct user acc;
	strcpy(acc.type,"学生");
	int mark=0; 
	printf("\n	请输入学生姓名 >>> "),scanf("%s",acc.name);
	mark=0;
	do{
		if(mark>0) printf("\n	该账号已存在，请修改后重新输入！！！\n"); 
		printf("\n	接下来请创建学生的【唯一】账号>>> "),scanf("%s",acc.acc);
		mark++;
	}while(acccfpd(acc.acc));
	printf("\n	输入并确认您的密码>>> "),scanf("%s",acc.pw);
// 保存账号信息到文件
    FILE *fp = fopen(strcmp(acc.type,"老师")==0 ? teaaccf : stuaccf, "ab");
    if (fp == NULL)
    {
        printf("\n	文件打开失败，注册失败！\n");
        return;
    }
    fwrite(&acc, sizeof(struct user), 1, fp); // 将用户信息写入文件
    fclose(fp);

    printf("\n	创建成功！即将返回上一级");
    Sleep(1000);
    system("cls");
}
void stuCTRLset(int mode) //学生密码重置函数 
{
	struct user temp;
	if(mode)
	{
		FILE *fp=fopen(stuaccf,"rb+");
		if (fp == NULL) // 文件不存在，验证失败
		{
			printf("文件不存在，验证失败\n");
	        exit(0);
	    }
        SetColor(11);
        printf("\n|===================[--学---生---个---人---密---码---重---置--]\n");
        SetColor(15);
        char act[30];
        int found,f=0;
        fseek(fp, 0, SEEK_SET);
        do{
            found=0;
            if(f) printf("[!] 账号不存在，请重新输入！");
            printf("\n请输入需要重置账号密码的学生的账号号>>> ");
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
    // 定义一个足够大的结构体数组来存储所有学生信息
    struct user students[maxusers];
    int studentCount = 0;

    // 打开文件读取所有学生信息
    FILE *fp = fopen(stuaccf, "rb");
    if (fp == NULL)
    {
        printf("文件打开失败，无法读取学生信息\n");
        return;
    }

    // 一次性将所有学生信息读入结构体数组
    while (studentCount < maxusers && fread(&students[studentCount], sizeof(struct user), 1, fp) == 1)
    {
        studentCount++;
    }
    fclose(fp);

    // 修改所有学生的密码为默认密码
    for (int i = 0; i < studentCount; i++)
    {
        strcpy(students[i].pw, mpw);
    }

    // 打开文件以覆盖写入模式
    fp = fopen(stuaccf, "wb");
    if (fp == NULL)
    {
        printf("文件打开失败，无法写入学生信息\n");
        return;
    }

    // 将修改后的学生信息写回文件
    for (int i = 0; i < studentCount; i++)
    {
        fwrite(&students[i], sizeof(struct user), 1, fp);
    }
    fclose(fp);
	}

    printf("\n    密码已重置为");
    SetColor(11);
    printf(mpw);
    SetColor(15);
    printf("\n    即将返回上一界面！\n");
    Sleep(2000);
    system("cls");
}
void stuCTRLdel() // 教师端学生信息管理删除学生函数 
{
    SetColor(11);
    printf("\n|===================[--学---生---账---号---删---除--]\n");
    SetColor(15);
    struct user students[maxusers];
    int studentCount = 0;
    // 打开文件读取所有学生信息
    FILE *fp = fopen(stuaccf, "rb");
    if (fp == NULL) {
        printf("文件打开失败，无法读取学生信息\n");
        return;
    }
    while (studentCount < maxusers && fread(&students[studentCount], sizeof(struct user), 1, fp) == 1) // 一次性将所有学生信息读入结构体数组
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
        if (f) printf("[!] 账号不存在，请重新输入！");
        printf("\n请输入需要删除的学生的账号号>>> ");
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
        printf("文件打开失败，无法写入学生信息\n");
        return;
    }
    for (int i = 0; i < studentCount; i++) {
        fwrite(&students[i], sizeof(struct user), 1, fp);
    }
    fclose(fp);
    printf("\n    账号删除成功！\n    即将返回上一界面！\n");
    Sleep(2000);
    system("cls");
}
void stuCTRLlook()//查看学生账号库
{
	FILE *fp=fopen(stuaccf,"rb");
	if (fp == NULL) {
        printf("文件打开失败，无法读取学生信息\n");
        return;
    }
	struct user temp;
	while(fread(&temp,sizeof(struct user),1,fp)==1)
	{
		printf("账号：%s\n",temp.acc);
		printf("密码：%s\n",temp.pw);
		printf("姓名：%s\n",temp.name);
		printf("-------------------------------------------------------------------\n");
	} 
	fclose(fp);
	printf("\n按任意键返回>>> ");
	scanf("[^\n]",temp.acc);
	system("cls");
} 
void tealook()//教师端查询学生答题情况入口 
{
	SetColor(11);
	printf("\n|===================[--学---生---成---绩---查---询--]\n");
	SetColor(15); 
	printf("\n	[1]查看指定学生答题记录\n");
	printf("\n	[2]查看全部答题记录\n");
	printf("\n	[0]返回");
	int cho,m=0;
	do{
		if(m>0) printf("\n[!] 您似乎按错键了，请重新输入！！！\n");
		printf("\n输入您的操作>>> ");
		scanf("%d",&cho);
		getchar();
		m++; 
	}while(cho!=1&&cho!=2&&cho!=0);
	system("cls");
	if(cho==0) return;
	else if(cho==1) relookalone();
	else if(cho==2) relookall();
}
void relookalone()//查询单个人
{
	printf("\n	请输入学生姓名>>> ");
	char ac[30];
	scanf("%[^\n]",ac);
	getchar();
	FILE* fp=fopen(resf,"rb");
	struct res temp;
	while(fread(&temp,sizeof(struct res),1,fp)==1) if(strcmp(ac,temp.name)==0) 
	{
		printf("\n============================================================\n");
		printf("姓名：%s\n",temp.name);
		printf("分数：%d/%d\n",temp.sco,temp.man);
		printf("答题情况：\n");
		for(int i=0;i<temp.tinum;i++)
		{
			printf("	%s:",temp.tilist[i]);
			if(temp.torf[i]==1) printf("正确\n");
			else printf("错误\n"); 
		}
		printf("答题日期:%s",temp.date);
	}
	fclose(fp);
	printf("\n按任意键返回>>> ");
	scanf("[^\n]",ac);
	system("cls");
}
void relookall()//查询所有 
{
	FILE* fp=fopen(resf,"rb");
	struct res temp;
	while(fread(&temp,sizeof(struct res),1,fp)==1) 
	{
		printf("\n============================================================\n") ;
		printf("姓名：%s\n",temp.name);
		printf("分数：%d/%d\n",temp.sco,temp.man);
		printf("答题情况：\n");
		for(int i=0;i<temp.tinum;i++)
		{
			printf("	%s:",temp.tilist[i]);
			if(temp.torf[i]==1) printf("正确\n");
			else printf("错误\n"); 
		}
		printf("答题日期:%s",temp.date);
	}
	fclose(fp);
	printf("\n按任意键返回>>> ");
	scanf("[^\n]",temp.name);
	system("cls");
}
