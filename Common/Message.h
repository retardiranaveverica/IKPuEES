#pragma once
#include <string>

typedef struct datas {
	int godine;
	char prezime[20];
	char ime[20];
} datas_s;

typedef struct message {
	int id_process;
	datas_s datas;
	int duzina_poruke;
}message_s;