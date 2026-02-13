#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#define N 500
int count=0;
typedef struct stuff
{
	int id;
	char name[30];
	float basic;
	float allowance;
	float tax;
	float actual;
	
}STUFF;
void printffile(STUFF a[],int n);//打印文件内容
void readfile(STUFF a[]);//从文件上读内容
void writefile(STUFF a[]);//写文件内容
void sortByActualSalary(STUFF a[], int n);//工资排序
void TaxAndActual(STUFF a[],int n);//扣税和实发工资
void tongji(STUFF a[],int n);//统计工资数据
void  xiugai(STUFF a[]);//修改员工工资
void  chaxun(STUFF a[]);//查询员工信息
void showMenu();//菜单
void addstuff(STUFF a[]);//添加员工
void creatTable(STUFF a[]);
int isChineseChar(char *str);//校验是否为中文汉字
int isPositiveNumber(float num);//校验数值是否为正数

int main()
{
	char adminPassword[] = "qwe";
    char inputPassword[20];
    int attempts = 3; // 三次输入机会
    
    printf("=== 管理员登录验证 ===\n");
    
    // 非管理员直接退出
    printf("是否为管理员？(y/n): ");
    char isAdmin;
    scanf(" %c", &isAdmin);
    if (isAdmin != 'y' && isAdmin != 'Y') {
        printf("非管理员，程序退出。\n");
        return 0;
    }
    
    // 管理员密码验证
    while (attempts > 0) {
        printf("请输入管理员密码（剩余%d次机会）：", attempts);
        scanf("%s", inputPassword);
        
        if (strcmp(inputPassword, adminPassword) == 0) {
            printf("密码正确！欢迎管理员登录。\n");
            break;
        } else {
            attempts--;
            if (attempts > 0) {
                printf("密码错误，请重试。\n");
            } else {
                printf("密码错误次数过多，程序退出。\n");
                return 0; // 密码错误次数过多直接退出
            }
        }
    }
	
	STUFF a[N];
	readfile(a);
	showMenu();
	
	int choice;
	while(1){
		printf("请输入要进行操作的序号\n");
		scanf("%d",&choice);
		
		switch(choice)
		{
			case 1:printffile(a,count);break;
			case 2:TaxAndActual(a,count);break;
			case 3:sortByActualSalary(a, count);break;
			case 4:xiugai(a);break;
			case 5:chaxun(a);break;
			case 6:tongji(a,count);break;
			case 7:addstuff(a);break;
			case 0:printf("退出系统\n");goto END_LOOP; // 跳出外层循环
			default:printf("输入错误，请重新选择！\n");
		}
		
		printf("是否继续操作(Y/N)\n");
		getchar(); 
		char m;
		scanf("%c",&m);
		if(m == 'N' || m == 'n') {
			break;
		}
	}
END_LOOP:
	return 0;
}

// 校验是否为中文汉字
int isChineseChar(char *str) {
    int i = 0;
    if (str == NULL || strlen(str) == 0) {
        return 0; 
    }
    // 遍历字符串，检查每个字符是否为中文
    while (str[i] != '\0') {
        if (!( (unsigned char)str[i] >= 0xB0 && (unsigned char)str[i] <= 0xF7 
              && (unsigned char)str[i+1] >= 0xA1 && (unsigned char)str[i+1] <= 0xFE )) {
            return 0; 
        }
        i += 2; 
    }
    return 1; 
}

// 校验数值是否为正数
int isPositiveNumber(float num) {
    if (num > 0.000001) { 
        return 1;
    }
    return 0;
}

void printffile(STUFF a[],int n)
{
	int i;
	printf("工号\t姓名\t基本工资\t津贴\t扣税\t实发工资\n");
	for(i=0;i<n;i++)
	{
		printf("%d\t%s\t%.2f\t\t%.2f\t%.2f\t%.2f\n", 
               a[i].id, a[i].name, a[i].basic, a[i].allowance, a[i].tax, a[i].actual);
	}
}

//读取文件
void readfile(STUFF a[])
{
	FILE *fp;
	int i=0;
	fp=fopen("employee.txt","r");
	// 判断文件是否打开成功
	if(fp == NULL){
        printf("文件打开失败，将创建新文件！\n");
        return;
    }
    while(fscanf(fp,"%d %s %f %f %f %f",&a[i].id,a[i].name,&a[i].basic,&a[i].allowance,&a[i].tax,&a[i].actual)!=EOF)
    {
    	i++;
	}
	count =i;
	fclose(fp);
}

// 添加员工
void addstuff(STUFF a[])
{
	char s='y';
	while(s == 'y' || s == 'Y')
	{
		char name[30];
		float basic, allowance;
		// 1. 校验姓名（必须中文）
		while(1) {
			printf("请输入员工姓名（仅支持中文）：");
			scanf("%s", name);
			if (isChineseChar(name)) {
				strcpy(a[count].name, name);
				break;
			} else {
				printf("姓名必须为中文汉字，请重新输入！\n");
			}
		}
		
		// 2. 校验基本工资（必须正数）
		while(1) {
			printf("请输入基本工资（必须为正数）：");
			scanf("%f", &basic);
			if (isPositiveNumber(basic)) {
				a[count].basic = basic;
				break;
			} else {
				printf("基本工资必须为正数，请重新输入！\n");
			}
		}
		
		// 3. 校验津贴（必须正数）
		while(1) {
			printf("请输入津贴（必须为正数）：");
			scanf("%f", &allowance);
			if (isPositiveNumber(allowance)) {
				a[count].allowance = allowance;
				break;
			} else {
				printf("津贴必须为正数，请重新输入！\n");
			}
		}
		
		// 自动生成工号
		a[count].id = count + 1;
		
		// 计算扣税和实发工资
		float total = a[count].basic + a[count].allowance;
		if(total <= 3000){
		    a[count].tax = 0;
		}else if(total > 3000 && total <= 4000){
		    a[count].tax = (total - 3000) * 0.05;
		}else{
		    a[count].tax = (total - 3000) * 0.05 + (total - 4000) * 0.1;
		}
		a[count].actual = total - a[count].tax;
		
		// 写入文件
		FILE *fp = fopen("employee.txt","a+");
		if(fp == NULL){
		    printf("文件打开失败！\n");
		    return;
		}
		fprintf(fp,"%d %s %.2f %.2f %.2f %.2f\n",a[count].id,a[count].name,a[count].basic,a[count].allowance,a[count].tax,a[count].actual);
		fclose(fp);
		
		count++;
		printf("员工添加成功！当前总员工数：%d\n", count);
		
		// 询问是否继续添加
		printf("是否继续录入(y/n)：");
		getchar(); 
		scanf("%c", &s);
	}
}

void showMenu(){
    printf("\n**********职工工资管理系统**********\n");
    printf("1. 查看职工工资表\n");
    printf("2. 计算职工扣税和实发工资\n");
    printf("3. 实发工资从高到低排序\n");
    printf("4. 修改职工工资信息\n");
    printf("5. 查询职工工资信息\n");
    printf("6. 职工工资统计分析\n");
    printf("7. 添加员工\n");
    printf("0. 退出系统\n");
    printf("************************************\n");
}

// 统计工资
void tongji(STUFF a[], int n) {
    if (n == 0) {
        printf("暂无职工信息，无法统计！\n");
        return;
    }

    int e = 0, b = 0, c= 0, d = 0;
    float sum = 0, avge, ratio1, ratio2, ratio3;

    // 统计工资区间与总和
    for (int i = 0; i < n; i++) {
        sum += a[i].actual;
        if (a[i].actual < 3000) {
            e++;
        } else if (a[i].actual >= 3000 && a[i].actual < 4000) {
            b++;
        } else {
            c++;
        }
    }

    // 计算平均值与比例
    avge= sum / n;
    ratio1 = (float)e / n;
    ratio2 = (float)b / n;
    ratio3 = (float)c / n;

    // 统计不低于平均工资的人数
    for (int i = 0; i < n; i++) {
        if (a[i].actual >= avge) {
            d++;
        }
    }

    // 输出统计结果
    printf("\n===== 工资统计分析报告 =====\n");
    printf("1. 工资 < 3000：人数 = %d，比例 = %.2f\n", e, ratio1);
    printf("2. 3000 ≤ 工资 < 4000：人数 = %d，比例 = %.2f\n", b, ratio2);
    printf("3. 工资 ≥ 4000：人数 = %d，比例 = %.2f\n", c, ratio3);
    printf("4. 平均工资：%.2f\n", avge);
    printf("5. 工资 ≥ 平均工资的人数：%d\n", d);
}

// 修改员工工资（增加正数校验）
void xiugai(STUFF a[]) {
    int target_id;
    float new_salary, new_allowance;

    printf("\n请输入要修改工资的员工ID：");
    scanf("%d", &target_id);

    // 查找目标员工
    int find_flag = 0;
    for (int i = 0; i < count; i++) {
        if (a[i].id == target_id) {
            find_flag = 1;
            
            // 校验新基本工资（正数）
            while(1) {
                printf("请输入新的基础工资（必须为正数）：");
                scanf("%f", &new_salary);
                if (isPositiveNumber(new_salary)) {
                    break;
                } else {
                    printf("基础工资必须为正数，请重新输入！\n");
                }
            }
            
            // 校验新津贴（正数）
            while(1) {
                printf("请输入新的津贴（必须为正数）：");
                scanf("%f", &new_allowance);
                if (isPositiveNumber(new_allowance)) {
                    break;
                } else {
                    printf("津贴必须为正数，请重新输入！\n");
                }
            }
            
            // 更新工资并重新计算扣税和实发工资
            a[i].basic = new_salary;
            a[i].allowance = new_allowance;
            float total = a[i].basic + a[i].allowance;
            if(total <= 3000){
                a[i].tax = 0;
            }else if(total > 3000 && total <= 4000){
                a[i].tax = (total - 3000) * 0.05;
            }else{
                a[i].tax = (total - 3000) * 0.05 + (total - 4000) * 0.1;
            }
            a[i].actual = total - a[i].tax;
            
            printf("修改成功！\n");
            printf("员工ID %d 新信息：基础工资=%.2f，津贴=%.2f，扣税=%.2f，实发工资=%.2f\n", 
                   target_id, new_salary, new_allowance, a[i].tax, a[i].actual);
            break;
        }
    }
    
    if (!find_flag) {
        printf("未找到员工ID：%d\n", target_id);
    }
}

// 查询员工信息
void chaxun(STUFF a[])
{
	int choice;
	printf("\n请选择查询方式：\n");
	printf("1. 按工号查询\n");
	printf("2. 按姓名查询\n");
	scanf("%d",&choice);
	
	if(choice == 1)
	{
		int id, flag = 0;
		printf("请输入要查询的职工ID：");
    	scanf("%d", &id);

    	for (int i = 0; i < count; i++) {
        	if (a[i].id == id) {
            	printf("\n===== 职工信息 =====\n");
            	printf("职工ID：%d\n", a[i].id);
            	printf("姓名：%s\n", a[i].name);
            	printf("基本工资：%.2f\n", a[i].basic);
            	printf("津贴：%.2f\n", a[i].allowance);
            	printf("税款：%.2f\n", a[i].tax);
            	printf("实发工资：%.2f\n", a[i].actual);
            	flag = 1;
            	break;
        	}
    	}

    	if (!flag) {
        	printf("未找到ID为%d的职工！\n", id);
    	}
	}
	else if(choice == 2)
	{
		char arr[30];
		printf("请输入要查询的姓名（中文）：");
		scanf("%s", arr);
		
		int flag = 0;
		for (int i = 0; i < count; i++) {
	        if (strcmp(a[i].name, arr) == 0) { // 精确匹配姓名
	            printf("\n===== 职工信息 =====\n");
	            printf("职工ID：%d\n", a[i].id);
	            printf("姓名：%s\n", a[i].name);
	            printf("基本工资：%.2f\n", a[i].basic);
	            printf("津贴：%.2f\n", a[i].allowance);
	            printf("税款：%.2f\n", a[i].tax);
	            printf("实发工资：%.2f\n", a[i].actual);
	            flag = 1;
	            break;
	        }
	    }
	    
	    if (!flag) {
	        printf("未找到姓名为%s的职工！\n", arr);
	    }
	}
	else {
		printf("输入错误，仅支持1或2！\n");
	}
}

// 计算扣税和实发工资
void TaxAndActual(STUFF a[],int n)
{
    if(n == 0){
        printf("未读入职工信息！\n");
        return;
    }
    
    float total;
    printf("工号\t姓名\t基本工资\t津贴\t扣税\t实发工资\n");
    // 计算扣税和实发工资
    for(int i=0; i<n; i++){
        total = a[i].basic + a[i].allowance;
        if(total <= 3000){
            a[i].tax = 0;
        }else if(total > 3000 && total <= 4000){
            a[i].tax = (total - 3000) * 0.05;
        }else{
            a[i].tax = (total - 3000) * 0.05 + (total - 4000) * 0.1;
        }
        a[i].actual = total - a[i].tax;
        
        printf("%d\t%s\t%.2f\t\t%.2f\t%.2f\t%.2f\n", 
               a[i].id, a[i].name, a[i].basic, a[i].allowance, a[i].tax, a[i].actual);
    }
}

// 按实发工资降序排序
void sortByActualSalary(STUFF a[], int n) {
    if (n == 0) {
        printf("暂无职工信息，无法排序！\n");
        return;
    }
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (a[j].actual < a[j+1].actual) {
                STUFF temp = a[j];
                a[j] = a[j+1];
                a[j+1] = temp;
            }
        }
    }
    
    printf("\n================= 实发工资从高到低排序 =================\n");
    printf("工号\t姓名\t基本工资\t津贴\t扣税\t实发工资\n");
    for(int k=0; k<n; k++)
    {
    	printf("%d\t%s\t%.2f\t\t%.2f\t%.2f\t%.2f\n", 
               a[k].id, a[k].name, a[k].basic, a[k].allowance, a[k].tax, a[k].actual);
	}
}