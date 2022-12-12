
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include <stdbool.h>
#include <Windows.h>


// 구조체 item 
struct ToDoItem {
	char* name;
	bool completed;
};

typedef struct ToDoItem* ToDoItemRef;

//구조체 list
struct ToDoList {
	int size;
	int capacity;
	ToDoItemRef* list;
};

typedef struct ToDoList* ToDoListRef;


void textcolor(int color_number)

{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}

void gotoxy(int x, int y)
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 콘솔창제어
void control_Console()
{
	system("mode con:cols=51");
	
}


void first() {
	gotoxy(17, 28);
	printf("처");
	Sleep(300);
	gotoxy(17, 28);
	printf("처음");
	Sleep(300);
	gotoxy(17, 28);
	printf("처음이");
	Sleep(300);
	gotoxy(17, 28);
	printf("처음이네");
	Sleep(300);
	gotoxy(17, 28);
	printf("처음이네요");
	Sleep(300);
	gotoxy(17, 31);
	printf("환");
	Sleep(300);
	gotoxy(17, 31);
	printf("환영");
	Sleep(300);
	gotoxy(17, 31);
	printf("환영합");
	Sleep(300);
	gotoxy(17, 31);
	printf("환영합니");
	Sleep(300);
	gotoxy(17, 31);
	printf("환영합니다");
	Sleep(300);
	
}

ToDoListRef ToDoListCreate() {

	ToDoListRef list = (ToDoListRef)malloc(sizeof(struct ToDoList));
	list->size = 0;
	list->capacity = 100;
	list->list = (ToDoItemRef*)malloc(sizeof(ToDoItemRef) * list->capacity);
	return list;
}

void DisplayToDoList(ToDoListRef list) {
	int i = 0;
	ToDoItemRef currentItem;
	/*
	textcolor(17);

	printf("\nㅁㅁㅁㅁ");
	textcolor(15);
	printf("To Do List");
	textcolor(17);
	printf("ㅁㅁㅁㅁ\n\n");
	textcolor(15);
	*/
	while (i < list->size) {
		textcolor(15);
		currentItem = (list->list)[i];
		if (currentItem->completed) {
			textcolor(8);
		}

		//char done = (currentItem->completed) ? 'X' : ' ';
		printf("\t%d: %s \n", i + 1, currentItem->name);
		i++;
		textcolor(15);
	}

}

ToDoItemRef ToDoItemCreate(char* name) {
	ToDoItemRef newItem = (ToDoItemRef)malloc(sizeof(struct ToDoItem));
	newItem->name = (char*)calloc(sizeof(char), strlen(name) + 1);
	strcpy(newItem->name, name);
	newItem->completed = false;
	return newItem;
}

ToDoItemRef ToDoListAddNewItem(ToDoListRef list, char* name) {
	if (list->size == list->capacity) {
		return NULL;
	}
	else {
		ToDoItemRef newItem = ToDoItemCreate(name);
		(list->list)[list->size] = newItem;
		list->size += 1;
		return newItem;
	}
}

void TodoItemRelease(ToDoItemRef item) {
	free(item->name);
	free(item);
}

int Add_Item(ToDoListRef list) {
	char name[1024];
	memset(name, 0, 1024);
	printf("\n 할 일은? : ");
	rewind(stdin);

	
	gets(name);
	
	
	ToDoListAddNewItem(list, name);
}


void ToDoItemRename(ToDoItemRef item, char* name) {


	if (strlen(item->name) < strlen(name)) {
		item->name = (char*)realloc(item->name, sizeof(char) * (strlen(name) + 1));

	}
	memset(item->name, 0, strlen(item->name) + 1);
	strcpy(item->name, name);

}

void Rename_Item(ToDoListRef list) {
	int i;
	ToDoItemRef selected;
	bool bool_selected = false;
	char name[1024];
	memset(name, 0, 1024);
	while (!bool_selected) {
		printf("\n수정할 번호를 입력해주세요 : ");
		
		while (!bool_selected) {
				scanf_s("%d", &i);
			if ((list->list)[i - 1]->completed == true) {
				printf("완료한 일은 수정할수 없습니다.");
				continue;
			}
			bool_selected = true;
		}

		if (i <= list->size) {
			bool_selected = true;
		}

		selected = (list->list)[i - 1];

		printf("\n변경할 제목을 입력해주세요 : ");
		rewind(stdin);
		gets(name);
		ToDoItemRename(selected, name);
	}
}

void TodoListRemoveItem(ToDoListRef list, int index) {
	if (index >= list->size || list->size == 0)
		return;

	ToDoItemRef toDel = (list->list)[index];
	int i = index;

	while (i < (list->size) - 1) {
		(list->list)[i] = (list->list)[i + 1];
		i++;
	}
	TodoItemRelease(toDel);
	list->size -= 1;

}

void Delete_Item(ToDoListRef list) {
	int i;
	bool bool_selected = false;
	while (!bool_selected) {
		printf("\n삭제 할 번호는? : ");
		scanf_s("%d", &i);
		if (i <= list->size)
			bool_selected = true;
		TodoListRemoveItem(list, i - 1);
	}
}

void Done_Item(ToDoListRef list) {
	ToDoItemRef selected;
	int i;
	bool bool_selected = false;
	while (!bool_selected) {
		printf("\n완료한 항목의 숫자를 입력해주세요");
		scanf_s("%d", &i);
		if (i <= list->size)
			bool_selected = true;
		selected = (list->list)[i - 1];
		selected->completed = true;
		printf("\nCOMPLTED.\n");

	}
}

void ToDoListAppendItem(ToDoListRef list, ToDoItemRef item) {
	if (list->size == list->capacity) return;
	(list->list)[list->size] = item;
	list->size += 1;
}

void ToDoListLoadFile(ToDoListRef list, FILE* file) {

	list->size = 0;
	int numberOfItems;
	int i = 0;
	ToDoItemRef newItem;
	int lengthOfTitle;
	bool completed;
	char* title;
	fread(&numberOfItems, sizeof(int), 1, file);
	while (i < numberOfItems) {

		fread(&lengthOfTitle, sizeof(int), 1, file);
		title = (char*)calloc(sizeof(char), lengthOfTitle + 1);
		fread(title, sizeof(char), lengthOfTitle, file);
		fread(&completed, sizeof(bool), 1, file);
		newItem = ToDoItemCreate(title);
		newItem->completed = completed;
		free(title);
		ToDoListAppendItem(list, newItem);
		i++;

	}

}

void ToDoListSaveFile(ToDoListRef list, FILE* file) {

	int numberOfItems = list->size;
	int lengthOfTitle;
	int capacityOfList = list->capacity;
	ToDoItemRef currentItem;
	fwrite(&numberOfItems, sizeof(unsigned int), 1, file);
	int i = 0;
	while (i < numberOfItems) {
		currentItem = (list->list)[i];
		lengthOfTitle = strlen(currentItem->name);
		fwrite(&lengthOfTitle, sizeof(int), 1, file);
		fwrite(currentItem->name, sizeof(char), lengthOfTitle, file);
		fwrite(&(currentItem->completed), sizeof(bool), 1, file);
		i++;
	}

}

void ToDoListRelase(ToDoListRef list) {
	int i = 0;

	while (i < list->size) {
		ToDoItemRef currentItem = (list->list)[i];
		TodoItemRelease(currentItem);
		i++;
	}
	free(list);
}



//종료함수
void exit_Program()
{
	gotoxy(10, 12);
	printf(" 잠시후 프로그램을 종료합니다.\n");
	Sleep(1000);
}

#define FILENAME ("todo.dat")

void start() {


	ToDoListRef list = ToDoListCreate();

	FILE* file;
	file = fopen(FILENAME, "rb");
	if (file) {
		ToDoListLoadFile(list, file);
		fclose(file);
	}
	else {
		first();
	}
	bool boolean = true;
	while (boolean) {
		system("cls");
		textcolor(15);
		printf("\n	              메 뉴  \n\n");
		printf(" ① 추 가 ");
		printf(" ② 수 정 ");
		printf(" ③ 삭 제 ");
		printf(" ④ 완 료 ");
		printf(" ⑤ 종 료 \n\n");
		textcolor(17);
		printf("ㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁ");
		textcolor(15);
		printf("To Do List");
		textcolor(17);
		printf("ㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁ\n\n");
		textcolor(15);


		textcolor(15);
		char n;
		printf("\n");
		DisplayToDoList(list);
		printf("\n\n▶▷▶▷▶▷");
		
		scanf_s("%c", &n);
		/*
		int c = 'a';
		printf("%d", c);
		*/
		
		switch (n)
		{
		case '1':
			//system("cls");
			Add_Item(list);
			//DisplayToDoList(list);
			system("cls");
			break;
		case '2':
			//system("cls");
			//DisplayToDoList(list);
			Rename_Item(list);
			system("cls");

			break;
		case '3':
			
			//DisplayToDoList(list);
			Delete_Item(list);
			system("cls");
			break;
		case '4':
			
			Done_Item(list);
			system("cls");
			break;
		case '5':
			system("cls");

			exit_Program();
			boolean = false;
			break;
		default: printf("숫자만 쓰세요 ㅡㅡ");
			break;
		} 
		
	}

	file = fopen("todo.dat", "wb");
	ToDoListSaveFile(list, file);
	ToDoListRelase(list);
	fclose(file);

}

int main() {
	control_Console();
	start();

	return 0;
}