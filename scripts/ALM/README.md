# ALM (Arduino Libraries Manager)

This folder contains the python scripts of Arduino Libraries Manager

## Arduino Library Installer

```shell
python .\arduino-library-installer.py --library 'Audio' --arduino-sdk C:\Users\92036\Desktop\sdk --lib-path C:\Users\92036\Desktop\pkgs --library-version '1.0.2' -f
```

- `--library-version` is optional (equals to `--library-version version-latest`). If you don't fill in this parameter, it will download the latest version of the library.
- `--library-version latest` means download the latest code of the library.
- `-f` is optional. If you add `-f`, it means the script will delete the exist library and download it again.

