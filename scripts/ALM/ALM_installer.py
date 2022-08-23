import json
import requests
import os
import sys
import shutil
import gzip
import zipfile
from distutils.version import LooseVersion

def gunzip_shutil(source_filepath, dest_filepath, block_size=65536):
    with gzip.open(source_filepath, 'rb') as s_file, \
            open(dest_filepath, 'wb') as d_file:
        shutil.copyfileobj(s_file, d_file, block_size)

class arduinoLibraryInstaller():
    _download_path = '/staging/libraries/'

    def __init__(self, arduinoSdk):
        self.arduinoSdkPath = arduinoSdk
        if(not os.path.isdir(self.arduinoSdkPath + self._download_path)):
            os.makedirs(self.arduinoSdkPath + self._download_path)

    def get_library_index(self):
        if os.path.exists(self.arduinoSdkPath + '/library_index.json') == False:
            file = requests.get('https://downloads.arduino.cc/libraries/library_index.json.gz').content
            with open(str(self.arduinoSdkPath + '/library_index.json.gz'), 'wb') as sdkJsonFile:
                sdkJsonFile.write(file)
            gunzip_shutil(self.arduinoSdkPath + '/library_index.json.gz', self.arduinoSdkPath + '/library_index.json')
            os.remove(self.arduinoSdkPath + '/library_index.json.gz')
        with open(self.arduinoSdkPath + '/library_index.json', 'rb') as sdkJsonFile:
            self.library_index = json.loads(sdkJsonFile.read())

    def find_library(self, libraryName, libraryVersion):
        if libraryVersion == 'latest':
            return None
        elif libraryVersion == 'version-latest':
            detectedLibrary = list(filter(lambda x: x['name'] == libraryName, self.library_index['libraries']))
        else:
            detectedLibrary = list(filter(lambda x: x['name'] == libraryName and x['version'] == libraryVersion, self.library_index['libraries']))

        if(len(detectedLibrary) == 0):
            print('Cannot find any library!')
            exit(1)
        elif(len(detectedLibrary) > 1): # find the latest version among the detectedLibrary
            max_version = LooseVersion(detectedLibrary[0]['version'])
            for i in range(len(detectedLibrary)):
                if max_version < LooseVersion(detectedLibrary[i]['version']):
                    max_version = LooseVersion(detectedLibrary[i]['version'])
            for i in range(len(detectedLibrary)):
                if max_version == LooseVersion(detectedLibrary[i]['version']):
                    break
            return detectedLibrary[i]
        else: # len(detectedLibrary) == 1
            return detectedLibrary[0]

    def download(self, library,forcely=False):
        url = library['url']
        if os.path.exists(self.arduinoSdkPath + self._download_path + library['archiveFileName']) == True:
            if forcely == True:
                os.remove(self.arduinoSdkPath + self._download_path + library['archiveFileName'])
            else:
                return

        print('[*] Downloading ' + library['archiveFileName'])
        with open(self.arduinoSdkPath + self._download_path + library['archiveFileName'], 'wb') as f:
            response = requests.get(url, stream=True)
            total = response.headers.get('content-length')

            if total is None:
                f.write(response.content)
            else:
                downloaded = 0
                total = int(total)
                for data in response.iter_content(chunk_size=max(int(total/1000), 1024*1024)):
                    downloaded += len(data)
                    f.write(data)
                    done = int(50*downloaded/total)
                    sys.stdout.write('\r[{}{}]'.format('=' * done, '.' * (50-done)))
                    sys.stdout.flush()
        sys.stdout.write('\n')
        print('[*] Done!')

    def extractArchive(self, library, destinationPath, forcely=False):
        destination_path = os.path.join(destinationPath, os.path.splitext(library['archiveFileName'])[0])
        if os.path.exists(destination_path) == True:
            if forcely == True:
                shutil.rmtree(destination_path)
            else:
                return

        print('[*] Extracting ' + library['archiveFileName'] + ' to ' + destinationPath)
        zipArchive = zipfile.ZipFile(self.arduinoSdkPath + self._download_path + library['archiveFileName'])
        zipArchive.extractall(destinationPath)
        print('[*] Done!')

    def download_latest_code(self, library, destinationPath, forcely=False):
         # TODO: use git to clone the latest code
        pass

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description='installer for arduino librarys with dependencies')
    parser.add_argument('--library', type=str, help='the library to install')
    parser.add_argument('--library-version', type=str, default='version-latest', help='the library version')
    parser.add_argument('--arduino-sdk', type=str, help='arduino sdk location')
    parser.add_argument('--lib-path', type=str, help='library output path')
    parser.add_argument('-f', action='store_true', default=False, help='download the library forcely')

    args = parser.parse_args()

    installer = arduinoLibraryInstaller(args.arduino_sdk)
    installer.get_library_index()

    lib = installer.find_library(args.library, args.library_version)
    if lib != None:
        installer.download(lib, args.f)
        installer.extractArchive(lib, args.lib_path, args.f)
    else:
        installer.download_latest_code(lib, args.lib_path, args.f)
