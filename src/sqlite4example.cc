/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sqlite4example.cc
 * Author: hansone123
 *
 * Created on May 9, 2016, 4:18 PM
 */

#include <cstdlib>
#include <sqlite4.h>
#include <string.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include <time.h>

#define BUFFER_SIZE 256

using namespace std;


static string makeInsertSQL(string tableName);
double genDoubleRandom(double fMin, double fMax);
string genStringRandom(const int maxLength);
int genIntegerRandom();
string genString(const int bytes);
void testInsertINT(const char *path, const int record_num);
void testInsertTEXT(const char *path, const int record_num, const int record_bytes);
/*
 * 
 */
int main(int argc, char** argv) {
    
    const char *databasePath;
    if (argc > 2) {
        databasePath = argv[1];
    } else {
        databasePath = "testDB-half-random-half-same-random-string";
    }
//    testInsertINT(databasePath, 1000000);
    testInsertTEXT(databasePath, 1000000, 1600);
//    string A = genString(1600);
//    string A = genStringRandom(1600);
//    cout<<A<<endl;
    return 0;
}
void testInsertTEXT(const char *path, const int record_num, const int record_bytes) {
    sqlite4 *db;
    
    char *ErrMsg = 0;
    int rc = sqlite4_open(0,path, &db);
//    sqlite4_env *env;
//    sqlite4_env_config();
    if( rc ){
       fprintf(stderr, "Can't open database: %s\n", sqlite4_errmsg(db));
        exit(0);
    }else{
       fprintf(stderr, "Opened database successfully\n");
    }
    
    char sSQL[BUFFER_SIZE];
    int used;
    cout<<"create table testTEXT."<<endl;
    sqlite4_exec(db, "CREATE TABLE testTEXT (col1 int primary key, col2 varchar)",NULL, NULL);
    cout<<"Prepared sql_stmt of INSERT."<<endl;
    sprintf(sSQL, "INSERT INTO testTEXT VALUES (@col1, @col2)");
//    string value = genString(record_bytes);
    clock_t cStartClock;
    cStartClock = clock();
    sqlite4_stmt *sql_stmt = NULL;
    sqlite4_prepare(db, sSQL, BUFFER_SIZE, &sql_stmt, &used);
    sqlite4_exec(db, "BEGIN TRANSACTION", NULL, NULL);
    cout<<"Bind sql_stmt."<<endl;
    string value_1 = genStringRandom(record_bytes/2);
    for (int i=0; i<record_num; i++) {
        
//        string value = genStringRandom(record_bytes/2) + genString(record_bytes/2);
        string value = value_1 + genStringRandom(record_bytes/2);
        sqlite4_bind_int(sql_stmt, 1, i);
        sqlite4_bind_text(sql_stmt, 2, value.c_str(), value.length(),0, 0);
        if (sqlite4_step(sql_stmt) != SQLITE4_DONE) {
            cout<<"Step failed."<<endl;
        }
        sqlite4_clear_bindings(sql_stmt);
        sqlite4_reset(sql_stmt);
    }
//    
    sqlite4_exec(db, "END TRANSACTION", NULL, NULL);
    printf("Imported %d records in %4.2f seconds\n", record_num, (clock() - cStartClock) / (double)CLOCKS_PER_SEC);
    sqlite4_finalize(sql_stmt);
//    sqlite4_exec(db, "DROP TABLE testTEXT",NULL, NULL);
    sqlite4_close(db, 0);
}
void testInsertINT(const char *path, const int record_num) {
    sqlite4 *db;
    
    char *ErrMsg = 0;
    int rc = sqlite4_open(0,path, &db);
    if( rc ){
       fprintf(stderr, "Can't open database: %s\n", sqlite4_errmsg(db));
        exit(0);
    }else{
       fprintf(stderr, "Opened database successfully\n");
    }
    
    char sSQL[BUFFER_SIZE];
    int used;
    cout<<"create table testInt."<<endl;
    sqlite4_exec(db, "CREATE TABLE testInt (col1 int, col2 int, col3 int,col4 int, col5 int)",NULL, NULL);
    cout<<"Prepared sql_stmt of INSERT."<<endl;
    sprintf(sSQL, "INSERT INTO testInt VALUES (@col1, @col2, @col3, @col4, @col5)");
    clock_t cStartClock;
    cStartClock = clock();
    sqlite4_stmt *sql_stmt = NULL;
    sqlite4_prepare(db, sSQL, BUFFER_SIZE, &sql_stmt, &used);
    sqlite4_exec(db, "BEGIN TRANSACTION", NULL, NULL);
    cout<<"Bind sql_stmt."<<endl;
   
    for (int i=0; i<record_num; i++) {
        int num = 1000;
        sqlite4_bind_int(sql_stmt, 1, num);
        sqlite4_bind_int(sql_stmt, 2, num);
        sqlite4_bind_int(sql_stmt, 3, num);
        sqlite4_bind_int(sql_stmt, 4, num);
        sqlite4_bind_int(sql_stmt, 5, num);
        if (sqlite4_step(sql_stmt) != SQLITE4_DONE) {
            cout<<"Step failed."<<endl;
        }
        sqlite4_clear_bindings(sql_stmt);
        sqlite4_reset(sql_stmt);
    }
//    
    sqlite4_exec(db, "END TRANSACTION", NULL, NULL);
    printf("Imported %d records in %4.2f seconds\n", record_num, (clock() - cStartClock) / (double)CLOCKS_PER_SEC);
    sqlite4_finalize(sql_stmt);
    sqlite4_exec(db, "DROP TABLE testInt",NULL, NULL);
    sqlite4_close(db, 0);
}
int genIntegerRandom()
{
    srand(time(NULL));
    return (int)rand();
}
string makeInsertSQL(string tableName) {
    string SQL = "insert into ";
    SQL.append(tableName);
    SQL.append(" values (");
    SQL.append(");");
    
    return SQL;
}
double genDoubleRandom(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
string genString(const int bytes) {
    char *result = new char[bytes+1];
    memset(result, 'A',bytes);
    return string(result);
}
string genStringRandom(const int Length) {
    const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    char buffer[Length+1];
    for (int i = 0; i < Length; ++i) {
        buffer[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return string(buffer);;
    
}