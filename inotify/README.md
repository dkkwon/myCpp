
### Goal
- Inotify 활용한 File/Folder Event 체크

### 참고 사이트
- https://blog.naver.com/websearch/221075783079
- https://sonseungha.tistory.com/436
- https://www.thegeekstuff.com/2010/04/inotify-c-program-example/
- <inotify-with-signal 예제>
  - https://stackoverflow.com/questions/4078345/inotify-with-signal-implementation-linux-c
- <FCNTL 사용법>
  - https://reakwon.tistory.com/110
- <SIGIO 란?>
  - http://chammoru.egloos.com/v/4183377
  - https://iplusu.tistory.com/entry/%EB%B9%84%EB%8F%99%EA%B8%B0-%EC%9E%85%EC%B6%9C%EB%A0%A5%EC%8B%9C%EA%B7%B8%EB%84%90
- <Symbolic link>
  - https://jybaek.tistory.com/578
  - https://william-yeh.net/post/2019/06/inotify-in-containers/


### Test (1)
```
[100%] Linking CXX executable test
[100%] Built target test
create_inotifier - success:../watchedfolder


    wd = 1; mask = IN_ATTRIB IN_CLOSE_WRITE
        name = b.txt
    wd = 1; mask = IN_ATTRIB IN_CLOSE_WRITE
        name = b.txt
```

```
➜  watchedfolder ls -ail
합계 20
15077349 drwxrwxr-x 4 dk dk 4096  1월  8 11:41 .
15077284 drwxrwxr-x 5 dk dk 4096  1월  8 11:41 ..
15077351 -rw-rw-r-- 1 dk dk   12  1월  8 11:03 b.txt
15077378 lrwxrwxrwx 1 dk dk    6  1월  8 11:41 folder -> origin
15077352 drwxrwxr-x 2 dk dk 4096  1월  8 11:01 new
15077354 drwxrwxr-x 2 dk dk 4096  1월  8 11:01 origin
➜  watchedfolder ln -sfn origin folder
➜  watchedfolder ln -sfn new folder
```

### Test (2)
```
name[] mask[40000020] cookie[0] len[0] IN_OPEN
name[] mask[40000001] cookie[0] len[0] IN_ACCESS
name[] mask[40000010] cookie[0] len[0] IN_CLOSE_NOWRITE

// Here, change contents of watchedfolder/a.txt

name[a.txt] mask[20] cookie[0] len[16] IN_OPEN
name[a.txt] mask[1] cookie[0] len[16] IN_ACCESS
name[a.txt] mask[10] cookie[0] len[16] IN_CLOSE_NOWRITE
name[a.txt] mask[2] cookie[0] len[16] IN_MODIFY
name[a.txt] mask[20] cookie[0] len[16] IN_OPEN
name[a.txt] mask[2] cookie[0] len[16] IN_MODIFY
name[] mask[40000020] cookie[0] len[0] IN_OPEN
name[] mask[40000001] cookie[0] len[0] IN_ACCESS
name[] mask[40000010] cookie[0] len[0] IN_CLOSE_NOWRITE
name[] mask[40000020] cookie[0] len[0] IN_OPEN
name[] mask[40000001] cookie[0] len[0] IN_ACCESS
name[] mask[40000010] cookie[0] len[0] IN_CLOSE_NOWRITE
name[a.txt] mask[8] cookie[0] len[16] IN_CLOSE_WRITE
name[] mask[40000020] cookie[0] len[0] IN_OPEN
name[] mask[40000001] cookie[0] len[0] IN_ACCESS
name[] mask[40000010] cookie[0] len[0] IN_CLOSE_NOWRITE
name[] mask[40000020] cookie[0] len[0] IN_OPEN
name[] mask[40000010] cookie[0] len[0] IN_CLOSE_NOWRITE
name[] mask[40000020] cookie[0] len[0] IN_OPEN
name[] mask[40000001] cookie[0] len[0] IN_ACCESS
name[] mask[40000010] cookie[0] len[0] IN_CLOSE_NOWRITE
```
