/*

新垣绫濑的荷包蛋
GitHub:https://github.com/AyaseLL6/stuManager
Email:f68886@88.com
2024.5.27

第一次使用先选择教师身份录入学生或者选择添加演示用默认学生功能
教师账号teacher
教师密码password

*/

#define _CRT_SECURE_NO_WARNINGS
//忽略安全检查

#include <stdio.h>
#include <stdlib.h>
//引入qsort排序函数(快速排序)
#include <string.h>
//导入头文件

#define MAX_LENGTH 20
// 学生学号最大长度
#define MAX_NAME 100
// 学生姓名最大长度
#define MAX_SUBJECTS 5
// 科目数
#define TEACHER_NAME "teacher"
// 教师账号
#define TEACHER_PASSWORD "password"
// 教师密码
int globalSubjectIndex;
// 全局变量，用于存储当前排序所依据的科目索引

/*
加上typedf可以省去struct关键字，例如:
typedef struct Student
{
    int a;
}Stu;
如果没有typedef就必须用struct Student stu1;来声明，有的话可以直接Stu stu1;
*/

typedef struct Student {
    char id[MAX_LENGTH];
    // 学号
    char name[MAX_NAME];
    // 姓名
    int scores[MAX_SUBJECTS];
    // 成绩数组
    int total_score;
    // 总成绩
    struct Student* next;
    // 结构体变量构成链表 next指向下一个struct Student类型的数据，构成链表
} Student;
// 学生信息结构体

typedef struct {
    Student* head;
    // 定义一个结构体类型StudentList，该类型包含一个指向Student结构体的指针成员head
} StudentList;

typedef struct {
    char username[20];
    char password[20];
    // 教师信息结构体
} Teacher;

void initList(StudentList* list) {
    list->head = NULL;
    // 初始化空的StudentList链表,将其初始状态设置为空链表
}

void saveToFile(StudentList* list) {
    // 保存链表数据到文件中
    FILE* file = fopen("students.dat", "wb");
    // 以二进制写模式(wb)打开students.dat文件
    if (file == NULL) {
        // 权限问题或文件不存在
        printf("无法打开学生数据\n");
        return;
    }
    // 遍历链表并写入文件
    Student* current = list->head;
    // 初始化一个指针current，指向链表的头节点
    while (current != NULL) {
        // 循环遍历链表，直到current为NULL，即链表的末尾
        fwrite(current, sizeof(Student), 1, file);
        /*将当前节点的数据写入文件，参数：要写入的数据指针，每个写入块的大小（即一个Student结构体的大小）
        写入块的数量（即一次写一个Student结构体），目标文件指针*/
        current = current->next;
        // 将current移动到下一个节点
    }
    fclose(file);
    // 关闭文件，确保所有数据写入文件并释放文件资源
}

void loadFromFile(StudentList* list) {
    // 从文件中读取Student结构体实例，并将它们添加到链表的头部
    FILE* file = fopen("students.dat", "rb");
    // 以二进制读模式(rb)打开students.dat文件
    if (file == NULL) {
        printf("无法找到学生数据,请先选择教师身份并录入学生或者选择添加演示用默认学生功能\n");
        return;
    }
    Student student;
    while (fread(&student, sizeof(Student), 1, file)) {
        // 使用fread函数从file中读取一个Student结构体数据到student变量中，fread函数返回读取的元素数量，成功读取一个时返1，否则为0
        Student* newStudent = (Student*)malloc(sizeof(Student));
        // 动态分配内存并创建一个新的Student结构体实例
        *newStudent = student;
        // 复制数据 将student变量中的数据复制到新分配的Student结构体实例中
        newStudent->next = list->head;
        // 设置新节点的next指针指向当前链表的头节点list->head
        list->head = newStudent;
        // 将链表的头指针list->head更新为newStudent，即新节点成为链表的新的头节点
    }
    // 每次读取一个学生数据，都会创建一个新的Student节点并将其插入到链表头部，最终形成一个链表，其中文件中最后读取的学生位于链表的头部
    fclose(file);
}

Student* createStudent(const char* id, const char* name, int scores[]) {
    // 录入学生
    Student* newStudent = (Student*)malloc(sizeof(Student));
    // 分配内存并创建一个新的Student结构体实例
    strcpy(newStudent->id, id);
    strcpy(newStudent->name, name);
    newStudent->total_score = 0;
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        newStudent->scores[i] = scores[i];
        newStudent->total_score += scores[i];
        // 计算总分
    }
    newStudent->next = NULL;
    return newStudent;
}

void addStudent(StudentList* list, Student* student) {
    student->next = list->head;
    // 将student的next指针指向链表当前的头节点
    list->head = student;
    // 将list的head指针指向student，使其成为链表的新头节点
    saveToFile(list);
    // 保存链表数据到文件中
}

void printStudent(Student* student) {
    // 打印当前节点的学生信息
    printf("| %-18s | %-30s |", student->id, student->name);
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf(" %-6d |", student->scores[i]);
    }
    printf(" %-6d |\n", student->total_score);
}


void printHeader() {
    // 打印学生信息表格头部
    printf("+--------------------+--------------------------------+");
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("--------+");
    }
    printf("--------+\n");
    printf("| %-18s | %-30s |", "学号", "姓名");
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf(" 科目%-3d |", i + 1);
    }
    printf(" 总分  |\n");
    printf("+--------------------+--------------------------------+");
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("--------+");
    }
    printf("--------+\n");
}

void printStudents(StudentList* list) {
    // 打印学生信息表格主体
    Student* current = list->head;
    // 定义一个指针current并将其初始化为链表的头节点
    printHeader();
    // 打印学生信息表格头部
    while (current != NULL) {
        // 循环遍历链表，直到current为NULL，即链表的末尾
        printStudent(current);
        // 打印当前节点的学生信息
        current = current->next;
        // 将current移动到下一个节点
    }
    printf("+--------------------+--------------------------------+");
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("--------+");
    }
    printf("--------+\n");
}

Student* findStudentById(StudentList* list, const char* id) {
    // 用于在学生链表中根据学生学号查找特定学生，并返回该学生节点的指针
    Student* current = list->head;
    // 定义一个指针current并将其初始化为链表的头节点
    while (current != NULL) {
        // 循环遍历链表，直到current为NULL，即链表的末尾
        if (strcmp(current->id, id) == 0) {
            return current;
            // 找到了目标学生，返回当前节点的指针
        }
        current = current->next;
        // 将current移动到下一个节点
    }
    return NULL;
}

void deleteStudent(StudentList* list, const char* id) {
    // 删除指定学生
    Student* current = list->head;
    // 定义一个指针current并将其初始化为链表的头节点
    Student* prev = NULL;
    // 定义一个指针prev并初始化为NULL，用于跟踪当前节点的前一个节点
    while (current != NULL) {
        // 循环遍历链表，直到current为NULL，即链表的末尾
        // 比较当前节点的ID (current->id) 和目标ID (id)
        if (strcmp(current->id, id) == 0) {
            // 如果找到匹配的节点
            if (prev == NULL) {
                // 如果要删除的是头节点，即前一个节点为空
                list->head = current->next;
                // 更新头节点为current->next 将头节点设置为下一个节点
            }
            else {
                // 如果要删除的不是头节点
                prev->next = current->next;
                //将前一个节点的next指针指向当前节点的下一个节点current->next 直接跳过该节点
            }
            free(current);
            // 释放要删除的节点
            saveToFile(list);
            // 保存链表数据到文件中
            return;
        }
        prev = current;
        current = current->next;
    }
}

void modifyStudentScores(StudentList* list, const char* id, int scores[]) {
    // 修改原有学生分数
    Student* student = findStudentById(list, id);
    // 按照学号查找特定学生，获得该学生节点的指针
    if (student != NULL) {
        student->total_score = 0;
        for (int i = 0; i < MAX_SUBJECTS; i++) {
            student->scores[i] = scores[i];
            student->total_score += scores[i];
            // 重新计算总成绩
        }
        saveToFile(list);
        // 保存链表数据到文件中
    }
}

int compareById(const void* a, const void* b) {
    // 用于比较两个学生的学号大小，a和b是指向指向Student指针的指针 小于返回负值 相等返回零 大于返回正值
    return strcmp((*(Student**)a)->id, (*(Student**)b)->id);
}

int compareByTotalScore(const void* a, const void* b) {
    // 用于比较两个学生总成绩的大小，a和b是指向指向Student指针的指针，直接比较两个学生的总成绩返回差值
    return (*(Student**)b)->total_score - (*(Student**)a)->total_score;
}

int compareBySubjectScore(const void* a, const void* b) {
    // 用于比较两个学生在某个科目上的成绩，从大到小排序，a和b是指向指向Student指针的指针
    return (*(Student**)b)->scores[globalSubjectIndex] - (*(Student**)a)->scores[globalSubjectIndex];
}


void sortStudents(StudentList* list, int (*compare)(const void*, const void*)) {
    int count = 0;
    Student* current = list->head;
    // 定义一个指针current并将其初始化为链表的头节点
    while (current != NULL) {
        //遍历链表，统计节点总数
        count++;
        current = current->next;
    }
    // 如果节点数小于2，直接返回，因为无需排序
    if (count < 2) return;
    // 计算链表节点数

    Student** array = (Student**)malloc(count * sizeof(Student*));
    // 分配一个数组array，大小为链表节点数
    current = list->head;
    for (int i = 0; i < count; i++) {
        array[i] = current;
        // 将链表节点指针逐个存入数组
        current = current->next;
    }
    // 将链表转换为数组

    qsort(array, count, sizeof(Student*), compare);
    // 使用标准库函数qsort对数组进行排序 待排序数据组 元素个数 每个元素大小 比较函数指针，用于元素间的比较  

    list->head = array[0];
    // 更新链表头指针为排序后数组的第一个元素
    for (int i = 1; i < count; i++) {
        array[i - 1]->next = array[i];
        // 遍历数组，重建链表的 next 指针
    }
    // 确保最后一个节点的next指针为NULL
    array[count - 1]->next = NULL;
    // 将数组转换回链表

    free(array);
    // 释放动态分配的数组内存
}

int findStudentRank(StudentList* list, const char* id) {
    //查询学生总成绩排名
    Student* current = list->head;
    // 定义一个指针current并将其初始化为链表的头节点
    int count = 0;
    while (current != NULL) {
        count++;
        //遍历链表，统计节点总数
        current = current->next;
    }
    if (count < 2) return 1;
    // 如果节点数小于2，直接返回，因为无需排序

    Student** array = (Student**)malloc(count * sizeof(Student*));
    // 分配一个数组array，大小为链表节点数
    current = list->head;
    for (int i = 0; i < count; i++) {
        array[i] = current;
        // 将链表节点指针逐个存入数组
        current = current->next;
    }
    // 将链表转换为数组

    qsort(array, count, sizeof(Student*), compareByTotalScore);
    // 使用标准库函数qsort对数组进行排序

    int rank = 1;
    // 用于存储目标学生的排名
    for (int i = 0; i < count; i++) {
        // 循环遍历已经按总成绩排序的学生指针数组
        if (strcmp(array[i]->id, id) == 0) {
            //比较当前数组元素的学号和目标学生的学号
            rank = i + 1;
            //因为数组索引从0开始，而排名从1开始，所以需要加1
            break;
        }
    }

    free(array);
    // 释放用于存储学生指针的数组内存
    return rank;
}

void sortStudentsBySubject(StudentList* list, int subjectIndex) {
    globalSubjectIndex = subjectIndex;
    // 将传入的科目索引赋值给全局变量

    int count = 0;
    Student* current = list->head;
    // 从链表头开始遍历
    while (current != NULL) {
        count++;
        // 遍历整个链表，统计学生数量
        current = current->next;
    }
    if (count < 2) return;
    // 如果学生数量少于2，直接返回（无需排序）

    Student** array = (Student**)malloc(count * sizeof(Student*));
    // 分配一个数组用于存储学生指针
    current = list->head;
    // 重新从列表头开始遍历
    for (int i = 0; i < count; i++) {
        array[i] = current;
        // 将链表中的每个学生指针存入数组
        current = current->next;
    }

    qsort(array, count, sizeof(Student*), compareBySubjectScore);
    // 使用qsort函数对数组进行排序

    list->head = array[0];
    // 将链表头指向排序后的第一个学生
    for (int i = 1; i < count; i++) {
        array[i - 1]->next = array[i];
        // 重新链接链表，使之按照排序后的顺序链接
    }
    array[count - 1]->next = NULL;
    // 将最后一个学生的next指针设为NULL

    free(array);
    // 释放用于存储学生指针的数组内存
}



void findStudentsByName(StudentList* list, const char* name) {
    int found = 0;
    for (Student* curr = list->head; curr != NULL; curr = curr->next) {
        if (strstr(curr->name, name) != NULL) {
            if (!found) {
                printHeader();
            }
            printStudent(curr);
            found = 1;
        }
    }
    if (!found) {
        printf("无法找到学生 \"%s\"\n", name);
    }
}

void findStudentsByScoreRange(StudentList* list, int a, int b) {
    int found = 0;
    for (Student* curr = list->head; curr != NULL; curr = curr->next) {
        if (curr->total_score >= a && curr->total_score <= b) {
            if (!found) {
                printHeader();
            }
            printStudent(curr);
            found = 1;
        }
    }
    if (!found) {
        printf("总成绩中无法找到在 %d-%d 分段的学生\n", a, b);
    }
}

void findStudentsBySubjectScoreRange(StudentList* list, int index, int a, int b) {
    int found = 0;
    if (index < 1 || index > MAX_SUBJECTS) {
        printf("错误的科目索引%d (索引范围1-5)\n", index);
        return;
    }
    for (Student* curr = list->head; curr != NULL; curr = curr->next) {
        if (curr->scores[index - 1] >= a && curr->scores[index - 1] <= b) {
            if (!found) {
                printHeader();
            }
            printStudent(curr);
            found = 1;
        }
    }
    if (!found) {
        printf("在科目%d 中无法找到在 %d-%d 分段的学生\n", index, a, b);
    }
}


void handleCommand(StudentList* list, char* command) {
    if (strncmp(command, "find by name ", 13) == 0) {
        char name[MAX_NAME];
        sscanf(command + 13, "%s", name);
        findStudentsByName(list, name);
    }
    else if (strncmp(command, "find by score ", 14) == 0) {
        int a, b;
        sscanf(command + 14, "%d-%d", &a, &b);
        findStudentsByScoreRange(list, a, b);
    }
    else if (strncmp(command, "find by subject ", 16) == 0) {
        int index, a, b;
        sscanf(command + 16, "%d %d-%d", &index, &a, &b);
        findStudentsBySubjectScoreRange(list, index, a, b);
    }
    else {
        printf("错误指令\n");
    }
}




void clearInputBuffer() {
    // 清空输入缓冲区
    while (getchar() != '\n');
}
void waitClearScreen() {
    // 等待下一步操作并清屏
    printf("按任意键继续...\n");
    getchar();
    // 等待用户按下任意键
    system("cls");
    // 清空屏幕
}


void teacherMenu(StudentList* list) {
    int choice;
    while (1) {
    printf("教师主系统:\n");
    printf("1. 录入学生成绩\n");
    printf("2. 修改学生成绩\n");
    printf("3. 删除指定学生\n");
    printf("4. 查找指定学生\n");
    printf("5. 按学号/成绩查看\n");
    printf("6. 按单科成绩查看\n");
    printf("7. 添加演示用默认学生\n");
    printf("8. 指令模糊匹配学生\n");
    printf("9. 返回身份选择界面\n");
    printf("\n模糊查询指令使用帮助:\n1.find by name 坤  显示姓名中包含'坤'的学生\n2.find by score 300-350  显示总成绩在300-350区间的学生\n3.find by subject 5  60-80  显示指定科目(1 - 5)的成绩在60-80区间的学生");
    printf("\n\n输入您的操作选择: ");
        scanf("%d", &choice);
        if (choice == 9) {
            system("cls");
            break;
        }
        char id[20], name[MAX_NAME];
        int scores[MAX_SUBJECTS];
        char command[256];
        Student* student;
        int s1[] = { 65,65,70,68,62 };
        int s2[] = { 75,69,77,82,70 };
        int s3[] = { 70,71,69,68,65 };
        int s4[] = { 100,110,95,108,107 };
        switch (choice) {
        case 1:
            // 录入新学生
            printf("输入学生学号 姓名 成绩(5科，空格分开): ");
            scanf("%s %s", id, name);
            for (int i = 0; i < MAX_SUBJECTS; i++) {
                scanf("%d", &scores[i]);
            }
            clearInputBuffer();
            student = createStudent(id, name, scores);
            // 创建新的学生节点
            addStudent(list, student);
            // 在链表头部插入新节点
            printf("录入成功\n");
            break;
        case 2:
            // 修改原有学生
            printf("输入学生学号: ");
            scanf("%s", id);
            printf("输入新的分数: ");
            for (int i = 0; i < MAX_SUBJECTS; i++) {
                scanf("%d", &scores[i]);
            }
            clearInputBuffer();
            modifyStudentScores(list, id, scores);
            // 修改原有特定学生的分数
            printf("修改成功\n");
            break;
        case 3:
            // 删除原有学生
            printf("输入学生学号: ");
            scanf("%s", id);
            clearInputBuffer();
            deleteStudent(list, id);
            // 删除指定学生
            printf("删除成功\n");
            break;
        case 4:
            // 查找指定学号学生
            printf("输入学生学号: ");
            scanf("%s", id);
            clearInputBuffer();
            student = findStudentById(list, id);
            // 按照学号查找特定学生，获得该学生节点的指针
            if (student) {
                printHeader();
                // 打印学生信息表格头部
                printStudent(student);
                // 打印学生信息表格主体
            }
            else {
                printf("该学生不存在\n");
            }
            break;
        case 5:
            printf("排序方式:\n");
            printf("1. 学号\n");
            printf("2. 总成绩\n");
            printf("输入您的选择: ");
            int sort_choice;
            scanf("%d", &sort_choice);
            clearInputBuffer();
            if (sort_choice == 1) {
                sortStudents(list, compareById);
                // 按照学号排序
            }
            else if (sort_choice == 2) {
                sortStudents(list, compareByTotalScore);
                // 按照总成绩排序
            }
            printStudents(list);
            // 排序完成后打印所有学生成绩
            break;
        case 6:
            // 按照单科成绩排序
            printf("选择科目 (1-5): ");
            int subjectIndex;
            scanf("%d", &subjectIndex);
            clearInputBuffer();
            if (subjectIndex >= 1 && subjectIndex <= 5) {
                sortStudentsBySubject(list, subjectIndex - 1);
                printStudents(list);
            }
            else {
                printf("无效科目选择\n");
            }
            break;
        case 7:
            
            student = createStudent("a", "坤哥", s1);
            addStudent(list, student);
            student = createStudent("b","熊大",  s2);
            addStudent(list, student);
            student = createStudent("c","熊二", s3);
            addStudent(list, student);
            student = createStudent("c","新垣绫濑", s3);
            addStudent(list, student);
            printf("添加成功\n");
            clearInputBuffer();
            break;
        case 8:
            clearInputBuffer();
                printf("输入指令: ");
                if (!fgets(command, sizeof(command), stdin)) {
                    break;
                }
                command[strcspn(command, "\n")] = 0;
                handleCommand(list, command);
            break;
        }
        waitClearScreen();
        // 等待下一步操作并清屏
    }
}

void studentMenu(StudentList* list) {
    int choice;
    while (1) {
        printf("学生主系统:\n");
        printf("1. 查询我的分数\n");
        printf("2. 查询我的排名(按总成绩)\n");
        printf("3. 返回身份选择界面\n");
        printf("输入您的选择: ");
        scanf("%d", &choice);
        if (choice == 3) {
            system("cls");
            break;
        }
        char id[20];
        Student* student;
        switch (choice) {
        case 1:
            printf("输入你的学号: ");
            scanf("%s", id);
            clearInputBuffer();
            student = findStudentById(list, id);
            // 按照学号查找特定学生，获得该学生节点的指针
            if (student) {
                printHeader();
                printStudent(student);
                // 打印学生信息
            }
            else {
                printf("该学生不存在\n");
            }
            break;
        case 2:
            printf("输入你的学号: ");
            scanf("%s", id);
            clearInputBuffer();
            student = findStudentById(list, id);
            if (student) {
                int rank = findStudentRank(list, id);
                printf("你的排名是: %d\n", rank);
            }
            else {
                printf("该学生不存在\n");
            }
            break;
        }
        waitClearScreen();
        // 等待下一步操作并清屏
    }
}

int main() {
    StudentList list;
    // 创建一个StudentList对象
    initList(&list);
    // 初始化为空链表
    loadFromFile(&list);
    // 从文件中加载数据填充到链表中

    int userType;
    while (1) {
        printf("身份选择:\n");
        printf("1. 教师身份\n");
        printf("2. 学生身份\n");
        printf("3. 退出系统\n");
        printf("输入您的选择: ");
        scanf("%d", &userType);
        if (userType == 3) break;
        system("cls");
        // 清屏

        if (userType == 1) {
            char username[20], password[20];
            printf("账号: ");
            scanf("%s", username);
            printf("密码: ");
            scanf("%s", password);
            system("cls");
            // 清屏

            Teacher teacher = { TEACHER_NAME,TEACHER_PASSWORD };
            if (strcmp(username, teacher.username) == 0 && strcmp(password, teacher.password) == 0) {
                // 验证教师身份
                teacherMenu(&list);
                // 将学生信息链表地址传入并显示教师主系统
            }
            else {
                printf("验证失败,请重试.\n");
            }
        }
        else if (userType == 2) {
            studentMenu(&list);
            // 将学生信息链表地址传入并显示教师主系统
        }
        else {
            printf("错误选择\n");
        }
    }


    saveToFile(&list);
    // 保存链表数据到文件中
    Student* current = list.head;
    // 定义一个指针current并将其初始化为链表的头节点
    while (current != NULL) {
        // 遍历并释放链表节点
        Student* temp = current;
        // 使用temp临时保存当前节点的指针
        current = current->next;
        // 将current更新为下一个节点
        free(temp);
        // 释放当前节点的内存
    }

    return 0;
}
