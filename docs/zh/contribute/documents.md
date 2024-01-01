# 如何增改文档

## 1 Docsify 简介

RTduino 使用 Docsify 来组织文档。Docsify 是一个轻量级的文档网站生成器，可以将 Markdown 文件转换为漂亮的网页。

## 2 如何构建本地文档网页

### 2.1 安装Node.js

<!-- tabs:start -->

#### ** Windows 10 **

1. 在浏览器中打开 Node.js 官方网站：https://nodejs.org/

2. 点击下载按钮，选择适用于 Windows 的 LTS 版本。

3. 下载完成后，双击安装程序并按照提示进行安装。

#### ** Ubuntu **

1. 打开终端。

2. 运行以下命令来安装 Node.js：

    ```bash
    sudo apt update
    sudo apt install nodejs
    ```

3. 安装完成后，运行以下命令来安装 npm：

    ```bash
    sudo apt install npm
    ```

4. 使用以下命令来验证安装是否成功：

    ```bash
    node -v
    npm -v
    ```

<!-- tabs:end -->

### 2.2 安装Docsify

要在本地构建Docisfy文档网页，可以按照以下步骤操作：

1. 首先，确保已经安装了 Node.js。可以在命令行中运行 `node -v` 来检查是否已安装。

2. 在命令行中，进入文档所在的目录。

3. 运行以下命令来安装 Docsify：

    ```bash
    npm install docsify-cli -g
    ```

4. 安装完成后，运行以下命令来启动本地服务器：

    ```bash
    docsify serve
    ```

5. 在浏览器中打开 `http://localhost:3000`，即可查看本地构建的文档网页。

现在，您可以根据需要对文档进行增改，并在本地实时预览效果。
