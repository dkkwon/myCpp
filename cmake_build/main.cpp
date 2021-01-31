#include <iostream>

int main(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++)  // 옵션의 개수만큼 반복
  {
    printf("%s\n", argv[i]);  // 옵션 문자열 출력
  }

  char* env1 = getenv("DK_ENV1");
  if (env1 != NULL) {
    printf("%d\n", atoi(env1));  // 옵션 문자열 출력
  }

  char* env2 = getenv("DK_ENV2");
  if (env2 != NULL) {
    printf("%s\n", env2);  // 옵션 문자열 출력
  }

  auto name = "it's me, dk";
  std::cout << "hello world: " << name << std::endl;
  return 0;
}
