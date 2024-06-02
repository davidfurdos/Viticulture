#include "header.h"

void initmatrix()
{
    matrix = (int **)calloc(7, sizeof(int *));
    for (int i = 0; i < 7; i++)
    {
        matrix[i] = (int *)calloc(capacities[i], sizeof(int));
    }
}

void initworkers()
{
    workers = malloc(maxworkers * sizeof(struct Worker));
    for (int i = 0; i < maxworkers; i++)
    {
        workers[i].id = 0;
        workers[i].name = strdup("szabad");
    }
}

void freematrix()
{
    for (int i = 0; i < 7; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void freeworkers()
{
    for (int i = 0; i < maxworkers; i++)
    {
        free(workers[i].name);
    }
    free(workers);
}

void readfromfile()
{
    char c;
    char buf[1024];
    int bufpos = 0;
    int m = 0;
    int n = 0;
    int capacitypos = 0;
    bool separator = false;
    int f = open("input.txt", O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
    while (read(f, &c, sizeof(c)))
    {
        if (m < 7)
        {
            if (c == ' ' || c == '\n')
            {
                buf[bufpos] = '\0';
                int num = atoi(buf);
                matrix[m][n] = num;
                n++;
                if (n == capacities[capacitypos])
                {
                    capacitypos++;
                    m++;
                    n = 0;
                }
                bufpos = 0;
            }
            else
            {
                buf[bufpos] = c;
                bufpos++;
            }
        }
        else
        {
            if (c == ' ' && !separator)
            {
                separator = true;
                buf[bufpos] = '\0';
                int num = atoi(buf);
                workers[num - 1].id = num;
                n = num - 1;
                bufpos = 0;
            }
            else if (c == '\n' && separator)
            {
                separator = false;
                buf[bufpos] = '\0';
                strcpy(workers[n].name, buf);
                bufpos = 0;
            }
            else
            {
                buf[bufpos] = c;
                bufpos++;
            }
        }
    }
    if (bufpos > 0 && separator)
    {
        buf[bufpos] = '\0';
        strcpy(workers[n].name, buf);
    }
    close(f);
}

void writetofile()
{
    int f = open("input.txt", O_WRONLY | O_TRUNC);
    int workernum = 0;
    for (int i = 0; i < maxworkers; i++)
    {
        if (workers[i].id != 0)
        {
            workernum++;
        }
    }
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < capacities[i]; j++)
        {
            char buffer[1024];
            sprintf(buffer, "%d", matrix[i][j]);
            write(f, buffer, strlen(buffer));
            if (j != capacities[i] - 1)
            {
                write(f, " ", 1);
            }
        }
        write(f, "\n", 1);
    }
    for (int i = 0; i < workernum; i++)
    {
        if (workers[i].id != 0)
        {
            char buffer[1024];
            sprintf(buffer, "%d %s", workers[i].id, workers[i].name);
            write(f, buffer, strlen(buffer));
            if (i != workernum - 1)
            {
                write(f, "\n", 1);
            }
        }
    }
    close(f);
    printf("Adatok mentve.\n");
}

int getnum(int numlength, int min, int max)
{
    int readmenu = 0;
    char *checkmenu = NULL;
    size_t len = 0;
    int num = min - 1;
    do
    {
        printf("Válasz: ");
        readmenu = getline(&checkmenu, &len, stdin);
        if (readmenu == -1)
        {
            printf("Hiba a bemenet olvasása során.\n");
            exit(1);
        }
        if (isdigit(*checkmenu) && atoi(checkmenu) == 0)
        {
            free(checkmenu);
            return 0;
        }
        if (readmenu > numlength + 1 || !isdigit(*checkmenu) || atoi(checkmenu) > max || atoi(checkmenu) < min)
        {
            printf("%d és %d közötti számot adjon meg.\n", min, max);
        }
        else
        {
            num = atoi(checkmenu);
        }
        free(checkmenu);
        checkmenu = NULL;
        len = 0;
    } while (num == min - 1);
    return num;
}

int checkday(char *input)
{
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == ' ' || input[i] == '\n')
        {
            input[i] = '\0';
        }
    }
    if (strcasecmp(input, "0") == 0)
    {
        return -2;
    }
    if (strcasecmp(input, "hétfő") == 0 || strcasecmp(input, "hetfo") == 0)
    {
        return 0;
    }
    else if (strcasecmp(input, "kedd") == 0)
    {
        return 1;
    }
    else if (strcasecmp(input, "szerda") == 0)
    {
        return 2;
    }
    else if (strcasecmp(input, "csütörtök") == 0 || strcasecmp(input, "csutortok") == 0)
    {
        return 3;
    }
    else if (strcasecmp(input, "péntek") == 0 || strcasecmp(input, "pentek") == 0)
    {
        return 4;
    }
    else if (strcasecmp(input, "szombat") == 0)
    {
        return 5;
    }
    else if (strcasecmp(input, "vasárnap") == 0)
    {
        return 6;
    }
    else
    {
        return -1;
    }
}

int countworkers()
{
    int counter = 0;
    for (int i = 0; i < maxworkers; i++)
    {
        if (workers[i].id != 0)
        {
            counter++;
        }
    }
    return counter;
}

void applytowork()
{
    int workercounter = 0;
    int freespacecounter = 0;
    int newid = 0;
    for (int i = 0; i < 7; i++)
    {
        if (workers[i].id != 0)
        {
            workercounter++;
        }
    }
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < capacities[i]; j++)
        {
            if (matrix[i][j] == 0)
            {
                freespacecounter++;
            }
        }
    }
    if (workercounter < maxworkers && freespacecounter > 0)
    {
        printf("Adja meg a nevét: ");
        getline(&name, &len, stdin);
        if (strcmp(name, "0\n") == 0)
        {
            printf("\nVissza\n");
            return;
        }
        if (name[strlen(name) - 1] == '\n')
        {
            name[strlen(name) - 1] = '\0';
        }
        for (int i = 0; i < maxworkers; i++)
        {
            if (workers[i].id == 0)
            {
                newid = i + 1;
                workers[i].id = newid;
                strcpy(workers[i].name, name);
                break;
            }
        }
        setshift(newid);
        printf("A dolgozó azonosítója: %d\nSikeres jelentkezés.\n", newid);
    }
    else
    {
        printf("Sajnos jelenleg nincs szabad hely.\n");
    }
}

void *setshift(int id)
{
    char buf[1024];
    int bufpos = 0;
    int *correctdays = malloc(7 * sizeof(int));
    int daycounter = 0;
    int errors = 0;
    int freespaces = 0;
    printf("Adja meg a napokat amiken dolgozni szeretne.\n");
    do
    {
        freespaces = 0;
        daycounter = 0;
        do
        {
            for (int i = 0; i < 7; i++)
            {
                correctdays[i] = 7;
            }
            errors = 0;
            daycounter = 0;
            printf("Válasz: ");
            getline(&readmenu, &len, stdin);
            if (strcmp(readmenu, "0\n") == 0)
            {
                printf("\nVissza\n");
                return 0;
            }
            for (int i = 0; i < 7; i++)
            {
                for (int j = 0; j < capacities[i]; j++)
                {
                    if (matrix[i][j] == id)
                    {
                        matrix[i][j] = 0;
                    }
                }
            }
            for (int i = 0; readmenu[i] != '\0'; i++)
            {
                char c = readmenu[i];
                if (c == ' ' || c == '\n')
                {
                    buf[bufpos] = '\0';
                    int len = strlen(buf);
                    int result = checkday(buf);
                    correctdays[daycounter] = result;
                    daycounter++;
                    bufpos = 0;
                }
                else
                {
                    buf[bufpos] = c;
                    bufpos++;
                }
            }
            if (bufpos > 0)
            {
                buf[bufpos] = '\0';
                int result = checkday(buf);
                correctdays[daycounter] = result;
                daycounter++;
                bufpos = 0;
            }

            for (int i = 0; i < 7; i++)
            {
                if (correctdays[i] == -1)
                {
                    errors++;
                }
            }
            if (errors > 0)
            {
                printf("Legalább 1 napot rosszul adott meg, kérem adja meg újra a kívánt beosztást:\n");
            }
        } while (errors > 0);
        for (int i = 0; i < 7; i++)
        {
            if (correctdays[i] >= 0 && correctdays[i] <= 6)
            {
                int bookedspaces = 0;
                for (int j = 0; j < capacities[correctdays[i]]; j++)
                {
                    if (matrix[correctdays[i]][j] == 0 || matrix[correctdays[i]][j] == id)
                    {
                        freespaces++;
                        break;
                    }
                    else
                    {
                        bookedspaces++;
                    }
                }
                if (bookedspaces == capacities[correctdays[i]])
                {
                    printf("%s\n", weekdaynames[correctdays[i]]);
                }
            }
        }
        if (freespaces == daycounter)
        {
            int match = 0;
            for (int i = 0; i < 7; i++)
            {
                if (correctdays[i] >= 0 && correctdays[i] <= 6)
                {
                    for (int j = 0; j < capacities[correctdays[i]]; j++)
                    {
                        if (matrix[correctdays[i]][j] == 0 || matrix[correctdays[i]][j] == id)
                        {
                            matrix[correctdays[i]][j] = id;
                            match++;
                            break;
                        }
                    }
                }
            }
            printf("\nSikeres beosztás foglalás.\n");
        }
        else
        {
            printf("\nA megadott beosztás nem megfelelő, mivel a fentebb említett napok nem szabadok, adjon meg egy másik beosztást.\n");
        }
    } while (freespaces != daycounter);
    free(correctdays);
}

int getworker(int id)
{
    for (int i = 0; i < maxworkers; i++)
    {
        if (workers[i].id == id)
        {
            return i + 1;
        }
    }
    return 0;
}

void changename(int id)
{
    printf("Adja meg az új nevet: ");
    getline(&name, &len, stdin);
    if (strcmp(name, "0\n") == 0)
    {
        printf("\nVissza\n");
        return;
    }
    if (name[strlen(name) - 1] == '\n')
    {
        name[strlen(name) - 1] = '\0';
    }
    for (int i = 0; i < maxworkers; i++)
    {
        if (workers[i].id == id)
        {
            strcpy(workers[i].name, name);
            printf("\nNév frissítve.\n");
            return;
        }
    }
    printf("Nincs ilyen azonsítóval rendelkező dolgozó.");
}

void deleteworker(int id)
{
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < capacities[i]; j++)
        {
            if (matrix[i][j] == id)
            {
                matrix[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < maxworkers; i++)
    {
        if (workers[i].id == id)
        {
            workers[i].id = 0;
            strcpy(workers[i].name, "szabad");
            printf("\nDolgozó törölve.\n");
            return;
        }
    }
    printf("\nHiba: A megadott azonosítóhoz nem tartozik dolgozó.\n");
}

void printdailyworkers(int index)
{
    printf("\nA %si napon dolgozók:\n", weekdaynames[index]);
    for (int i = 0; i < capacities[index]; i++)
    {
        if (matrix[index][i] == 0)
        {
            printf("szabad\n");
        }
        else
        {
            for (int j = 0; j < maxworkers; j++)
            {
                if (matrix[index][i] == workers[j].id)
                {
                    printf("%d - ", workers[j].id);
                    printf("%s\n", workers[j].name);
                }
            }
        }
    }
}

void printallworker()
{
    printf("\nAz összes jelentekző azonosítója és neve:\n");
    for (int i = 0; i < maxworkers; i++)
    {
        if (workers[i].id != 0)
        {
            printf("%d - ", workers[i].id);
            printf("%s\n", workers[i].name);
        }
    }
    printf("\nA jelenlegi beosztásuk:\n\n");
    for (int i = 0; i < 7; i++)
    {
        printf("%s:\n", weekdaynames[i]);
        for (int j = 0; j < capacities[i]; j++)
        {
            if (matrix[i][j] == 0)
            {
                printf("szabad\n");
            }
            else
            {
                for (int k = 0; k < maxworkers; k++)
                {
                    if (matrix[i][j] == workers[k].id)
                    {
                        printf("%d - ", workers[k].id);
                        printf("%s\n", workers[k].name);
                        break;
                    }
                }
            }
        }
        printf("\n");
    }
}

void handler(int signumber)
{
    if (signumber == SIGUSR1)
    {
        printf("\nAz első busz indulásra kész.\n");
    }
    else if (signumber == SIGUSR2)
    {
        printf("A második busz indulásra kész.\n");
    }
}

struct Worker *initworkersonbus(int passengers)
{
    struct Worker *localworkers = malloc(passengers * sizeof(struct Worker));
    for (int i = 0; i < 5; i++)
    {
        localworkers[i].id = 0;
        localworkers[i].name = strdup("szabad");
    }
    return localworkers;
}

struct Worker *getdailyworkers(int index)
{
    struct Worker *dailyworkers = malloc(capacities[index] * sizeof(struct Worker));
    printf("\nA %si napon dolgozók:\n", weekdaynames[index]);
    for (int i = 0; i < capacities[index]; i++)
    {
        if (matrix[index][i] == 0)
        {
            dailyworkers[i].id = 0;
            dailyworkers[i].name = "szabad";
        }
        for (int j = 0; j < maxworkers; j++)
        {
            if (matrix[index][i] == workers[j].id)
            {
                dailyworkers[i].id = workers[j].id;
                dailyworkers[i].name = workers[j].name;
            }
        }
    }
    return dailyworkers;
}

struct Worker *checkdailyworkers(int index, struct Worker *localworkers)
{
    struct Worker *dailyworkers = malloc(sizeof(struct Worker));
    int size = 0;
    for (int i = 0; i < capacities[index]; i++)
    {
        if (localworkers[i].id == 0)
        {
            continue;
        }
        else
        {
            size++;
            dailyworkers = realloc(dailyworkers, size * sizeof(struct Worker));
            dailyworkers[size - 1].id = localworkers[i].id;
            dailyworkers[size - 1].name = localworkers[i].name;
        }
    }
    return dailyworkers;
}

int getdailynumofworkers(int index, struct Worker *localworkers)
{
    int cnt = 0;
    for (int i = 0; i < capacities[index]; i++)
    {
        if (localworkers[i].id != 0)
        {
            cnt++;
        }
    }
    return cnt;
}

struct Worker *formatdailyworkers(int length, struct Worker *localworkers)
{
    struct Worker *dailyworkers = malloc(length * sizeof(struct Worker));
    int cnt = 0;
    while (cnt < length)
    {
        dailyworkers[cnt] = localworkers[cnt];
        cnt++;
    }
    for (int i = length; i < 10; i++)
    {
        dailyworkers[i].id = 0;
        dailyworkers[i].name = "szabad";
    }
    return dailyworkers;
}

int sendmsg(int messagequeue, long id, int numofpassengers)
{
    struct message msg = {id, numofpassengers};
    int status;
    status = msgsnd(messagequeue, &msg, sizeof(int), 0);
    if (status < 0)
        perror("Az üzenetet nem lehet elküldeni.\n");
    return 0;
}

int recivemsg(int messagequeue, long id)
{
    struct message msg;
    int status;
    status = msgrcv(messagequeue, &msg, sizeof(int), id, 0);
    if (status < 0)
        perror("Az üzenetet nem sikerült fogadni.\n");
    else
        printf("A(z) %li. busszal megérkezett dolgozók száma: %i\n", id, msg.passengernum);
    return 0;
}

void transportworkers(int dayindex, char *argv)
{
    pid_t bus1, bus2;
    int status;
    int fd1[2], fd2[2];
    key_t key;
    key = ftok(argv, 1);
    int messagequeue = msgget(key, 0600 | IPC_CREAT);

    if (pipe(fd1) == -1)
    {
        perror("Hiba az első cső létrehozásakor.\n");
        exit(1);
    }
    if (pipe(fd2) == -1)
    {
        perror("Hiba a második cső létrehozásakor.\n");
        exit(1);
    }
    if (messagequeue < 0)
    {
        perror("Hiba az üzenetsor létrehozásakor.\n");
        return;
    }

    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);

    bus1 = fork();
    if (bus1 < 0)
    {
        printf("Az első busz nem tudott elindulni.\n");
        exit(1);
    }
    if (bus1 == 0) // első gyerek
    {
        kill(getppid(), SIGUSR1);

        close(fd1[1]);

        int passengersbus1 = 0;
        read(fd1[0], &passengersbus1, sizeof(int));

        struct Worker *busworkers = initworkersonbus(passengersbus1);
        int cnt = 0;

        if (passengersbus1 > 0)
        {
            for (int i = 0; i < passengersbus1; i++)
            {
                read(fd1[0], &busworkers[i], sizeof(struct Worker));
                if (&busworkers[i] != NULL)
                {
                    cnt++;
                }
            }
            printf("Az első busszal szállított dolgozók:\n");
            for (int i = 0; i < passengersbus1; i++)
            {
                printf("%i - %s\n", busworkers[i].id, busworkers[i].name);
            }
        }

        close(fd1[0]);
        sleep(1);
        sendmsg(messagequeue, 1, cnt);
        if (messagequeue < 0)
        {
            perror("Az üzenetsorral kapcsolatban hiba lépett fel.\n");
            return;
        }

        exit(0);
    }
    else
    {
        bus2 = fork();
        if (bus2 < 0)
        {
            printf("A második busz nem tud elindulni.\n");
            exit(1);
        }
        if (bus2 == 0) // második gyerek
        {
            sleep(1);
            kill(getppid(), SIGUSR2);

            close(fd2[1]);

            int passengersbus2 = 0;
            read(fd2[0], &passengersbus2, sizeof(int));

            struct Worker *busworkers = initworkersonbus(passengersbus2);
            int cnt = 0;
            if (passengersbus2 > 0)
            {
                for (int i = 0; i < passengersbus2; i++)
                {
                    read(fd2[0], &busworkers[i], sizeof(struct Worker));
                    if (&busworkers[i] != NULL)
                    {
                        cnt++;
                    }
                }
                printf("A második busszal szállított dolgozók:\n");
                for (int i = 0; i < passengersbus2; i++)
                {
                    printf("%i - %s\n", busworkers[i].id, busworkers[i].name);
                }
            }

            close(fd2[0]);
            sleep(1);
            sendmsg(messagequeue, 2, cnt);
            if (messagequeue < 0)
            {
                perror("Az üzenetsorral kapcsolatban hiba lépett fel.\n");
                return;
            }

            exit(0);
        }
        else // szülő
        {
            pause();

            close(fd1[0]);
            close(fd2[0]);

            struct Worker *dailybusworkers = getdailyworkers(dayindex);
            dailybusworkers = checkdailyworkers(dayindex, dailybusworkers);
            int numofworkers = getdailynumofworkers(dayindex, dailybusworkers);
            int passengersbus1 = 0;
            int passengersbus2 = 0;

            if (numofworkers <= 5)
            {
                passengersbus1 = numofworkers;
            }
            else if (numofworkers > 5 && numofworkers <= 10)
            {
                passengersbus1 = 5;
                passengersbus2 = numofworkers - 5;
            }
            else if (numofworkers > 10)
            {
                passengersbus1 = 5;
                passengersbus2 = 5;
            }

            write(fd1[1], &passengersbus1, sizeof(int));
            write(fd2[1], &passengersbus2, sizeof(int));

            if (passengersbus1 > 0)
            {
                for (int i = 0; i < passengersbus1; i++)
                {
                    write(fd1[1], &dailybusworkers[i], sizeof(struct Worker));
                }
            }
            else if (passengersbus1 == 0)
            {
                printf("A mai napon egyik busz sem szállít dolgozókat.\n");
            }

            sleep(1);

            if (passengersbus2 > 0)
            {
                for (int i = passengersbus1; i < passengersbus1 + passengersbus2; i++)
                {
                    write(fd2[1], &dailybusworkers[i], sizeof(struct Worker));
                }
            }
            else if (passengersbus2 == 0)
            {
                printf("A mai napon a második busz nem szállít dolgozókat.\n");
            }

            close(fd1[1]);
            close(fd2[1]);

            waitpid(bus2, &status, 0);

            recivemsg(messagequeue, 1);
            recivemsg(messagequeue, 2);

            free(dailybusworkers);
        }
    }
}