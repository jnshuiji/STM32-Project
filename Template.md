# Template

    这是一个使用keil开发STM32103C8的模板工程。基于Keil MDK540版本，使用的库函数版本2.4.1来自于keil官网，安装于keil安装目录下。

## 库函数

    该模板使用**Run-Time Environment**来配置库函数的使用。

![image-20240701143259298](./.img/README/image-20240701143259298.png)

### 内核

![CMSIS.png](./.img/README/CMSIS.png)

### Device

    StdPeriph Drivers全选，GPIO未知。

![Device.png](./.img/README/Device.png)

## main

    默认组User，界面如下：

![2024-06-29-13-35-26-image.png](./.img/README/2024-06-29-13-35-26-image.png)

## 包含路径

    keil可以直接在其安装目录下找到头文件，而vscode不行，所以在keil中添加包含路径以便vscode可以跳转函数对应的头文件。

![2024-06-29-13-24-42-image.png](./.img/README/2024-06-29-13-24-42-image.png)

    以1.LED为例，效果如下：

![2024-06-29-13-26-43-image.png](./.img/README/2024-06-29-13-26-43-image.png)

## 调试

    使用ST-Link

![2024-06-29-13-29-15-image.png](./.img/README/2024-06-29-13-29-15-image.png)