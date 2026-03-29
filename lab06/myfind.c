#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <linux/limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

bool print(const char *const path);
void dfs(const char *const path);

int main(int argc, char *argv[])
{
  if (2 != argc)
  {
    fprintf(stderr, "wrong number of arguments");
    return 1;
  }

  struct stat def;
  if (-1 == stat(argv[1], &def))
  {
    perror("stat() failed");
    return 2;
  }

  dfs(argv[1]);
  return 0;
}

void dfs(const char *const path)
{
  // base case: not directory
  if (false == print(path))
    return;

  DIR *directory = opendir(path);
  if (NULL == directory)
  {
    perror("opendir() failed");
    exit(3);
  }

  char upgr_path[PATH_MAX];
  struct dirent *dir_data = NULL;
  while (dir_data = readdir(directory))
  {
    if (0 == strcmp(dir_data->d_name, ".") ||
        0 == strcmp(dir_data->d_name, ".."))
      continue;

    if (-1 == sprintf(upgr_path, "%s/%s", path, dir_data->d_name))
    {
      perror("sprintf() failed");
      exit(4);
    }

    dfs(upgr_path);
  }

  if (-1 == closedir(directory))
  {
    perror("closedir() failed");
    exit(5);
  }
}

bool print(const char *const path)
{
  struct stat st;
  struct passwd *pwd;
  struct group *grp;
  char perm[10] = "---------";
  int result = 0;

  if (0 != stat(path, &st))
  {
    fprintf(stderr, "Eroare la stat pentru %s .\t", path);
    perror("Cauza este");
    return 2;
  }

  printf("\tTipul acestui fisier: ");
  switch (st.st_mode & S_IFMT)
  {
  case S_IFDIR:
    printf("Director\n");
    result = 1;
    break;
  case S_IFREG:
    printf("Fisier obisnuit\n");
    break;
  case S_IFLNK:
    printf("Link\n");
    break;
  case S_IFIFO:
    printf("FIFO\n");
    break;
  case S_IFSOCK:
    printf("Socket\n");
    break;
  case S_IFBLK:
    printf("Block device\n");
    break;
  case S_IFCHR:
    printf("Character device\n");
    break;
  default:
    printf("Unknown file type");
  }

  printf("\tNumarul de nume alternative (i.e., link-uri hard) ale fisierului: %ld octeti\n", st.st_nlink);

  printf("\tDimensiunea acestuia: %lld octeti\n", (long long)st.st_size);
  printf("\tSpatiul ocupat pe disc pentru stocarea acestuia: %lld sectoare (blocuri de 4096 octeti).\n", (long long)st.st_blocks);

  printf("\tPermisiunile acestuia, in notatie octala: %o\n", st.st_mode & 0777);
  if (S_IRUSR & st.st_mode)
    perm[0] = 'r';
  if (S_IWUSR & st.st_mode)
    perm[1] = 'w';
  if (S_IXUSR & st.st_mode)
    perm[2] = 'x';
  if (S_IRGRP & st.st_mode)
    perm[3] = 'r';
  if (S_IWGRP & st.st_mode)
    perm[4] = 'w';
  if (S_IXGRP & st.st_mode)
    perm[5] = 'x';
  if (S_IROTH & st.st_mode)
    perm[6] = 'r';
  if (S_IWOTH & st.st_mode)
    perm[7] = 'w';
  if (S_IXOTH & st.st_mode)
    perm[8] = 'x';

  printf("\tPermisiunile acestuia, in notatie simbolica: %s\n", perm);

  if (NULL != (pwd = getpwuid(st.st_uid)))
    printf("\tProprietarul acestuia: %s (cu UID-ul: %ld)\n", pwd->pw_name, (long)st.st_uid);
  else
    printf("\tProprietarul acestuia are UID-ul: %ld\n", (long)st.st_uid);

  if (NULL != (grp = getgrgid(st.st_gid)))
    printf("\tGrupul proprietar al acestuia: %s (cu GID-ul: %ld)\n", grp->gr_name, (long)st.st_gid);
  else
    printf("\tGrupul proprietar al acestuia are GID-ul: %ld\n", (long)st.st_gid);

  return result;
}
