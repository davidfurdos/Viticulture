#include "szoleszet.c"

int main(int argc, char **argv)
{
    initmatrix();
    initworkers();
    int version;
    do
    {
        printf("\n-----------------------------------\n");
        printf("Válasszon a menüből:\n");
        printf("\n0. Vissza/kilépés\n");
        printf("\n1. Adatok kezelése\n2. Buszok indítása");
        printf("\n-----------------------------------\n");
        version = getnum(1, 0, 2);
        switch (version)
        {
        case 0:
            printf("Kilépés...\n");
            break;
        case 1:
            readfromfile();
            int menu;
            do
            {
                printf("\n-----------------------------------\n");
                printf("Válasszon a menüből:\n");
                printf("\n0. Vissza/kilépés\n");
                printf("\n1. Jelentkezés\n2. Adatok módosítása\n3. Adatok törlése\n4. Napi jelentkezők listája\n5. Jelentkezők listája");
                printf("\n-----------------------------------\n");
                menu = getnum(1, 0, 5);
                switch (menu)
                {
                case 0:
                    writetofile();
                    printf("Kilépés..\n");
                    break;
                case 1:
                    applytowork();
                    break;
                case 2:
                    printf("Kérem adja meg azon dolgozó azonosítóját, amelynek adatait módosítani szeretné.\n");
                    int modifyworkerid = getnum(2, 1, countworkers());
                    if (modifyworkerid == 0)
                    {
                        printf("\nVissza\n");
                        break;
                    }
                    else
                    {
                        int workerid = getworker(modifyworkerid);
                        if (workerid != 0)
                        {
                            printf("\n1. Név változtás\n2. Beosztás módosítás\n");
                            int modifyworkerdata = getnum(2, 1, 2);
                            if (modifyworkerdata == 0)
                            {
                                printf("\nVissza\n");
                                break;
                            }
                            else
                            {
                                if (modifyworkerdata == 1)
                                {
                                    changename(modifyworkerid);
                                }
                                else
                                {
                                    setshift(modifyworkerid);
                                }
                            }
                        }
                        else
                        {
                            printf("\nA megadott azonosítóval nem található dolgozó.\n");
                        }
                    }
                    break;
                case 3:
                    printf("Adja meg annak a dolgozónak az azonosítóját, amelynek adatait törölni szeretné.\n");
                    int deleteid = getnum(2, 1, countworkers());
                    if (deleteid == 0)
                    {
                        printf("\nVissza\n");
                        break;
                    }
                    else
                    {
                        deleteworker(deleteid);
                    }
                    break;
                case 4:
                    printf("Adja meg melyik nap dolgozóit szeretné kilistázni.\n");
                    int dailyindex;
                    do
                    {
                        dailyindex = -1;
                        printf("Válasz: ");
                        getline(&readmenu, &len, stdin);
                        if (strcmp(readmenu, "0\n") == 0)
                        {
                            printf("\nVissza\n");
                            break;
                        }
                        dailyindex = checkday(readmenu);
                        if (dailyindex == -1)
                        {
                            printf("Hibás napot adott meg, kérem adja meg újra:\n");
                        }
                    } while (dailyindex == -1);
                    if (dailyindex != -1)
                    {
                        printdailyworkers(dailyindex);
                    }
                    break;
                case 5:
                    printallworker();
                    break;
                default:
                    printf("Valamilyen hiba lépett fel.\n");
                    break;
                }
            } while (menu != 0);
            break;
        case 2:
            readfromfile();
            int transportindex;
            do
            {
                printf("\n-----------------------------------\n");
                printf("Adja meg mely nap járatát indítsuk:\n");
                printf("Válasz: ");
                getline(&readmenu, &len, stdin);
                transportindex = checkday(readmenu);
                if (transportindex == -1)
                {
                    printf("\nHibás napot adott meg, kérem adja meg újra.\n");
                }
                if (transportindex == -2)
                {
                    printf("\nVissza\n");
                }
                if (transportindex >= 0)
                {
                    transportworkers(transportindex, argv[0]);
                }
            } while (transportindex != -2);
            break;
        }
    } while (version != 0);
    free(readmenu);
    freematrix();
    freeworkers();
    return 0;
}