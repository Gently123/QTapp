# Windows Process Manager（任务管理器）

本项目是一个基于 Qt 框架开发的 Windows 平台进程管理器，模拟 Windows 任务管理器的部分核心功能。作为操作系统课程设计的一部分，旨在提升对系统调用、进程控制和 GUI 编程的综合掌握能力。

## ✨ 功能简介

- ✅ 实时获取并展示系统进程信息（多线程刷新）
- ✅ 显示字段包括：图标、进程名、PID、状态、CPU 占用率、内存使用、线程数、优先级
- ✅ 支持模糊搜索（按名称或 PID）
- ✅ 支持右键菜单快捷操作：结束进程、打开进程所在文件夹
- ✅ 简洁美观的 QSS 风格界面

## 📸 界面预览

> 可在此处插入项目运行截图，建议命名为 `screenshot.png`，并上传至仓库中

## 🛠️ 技术实现

- Qt Creator 4.0.3 + Qt 5.x
- 使用 Windows API 获取系统进程数据（如 ToolHelp32Snapshot、OpenProcess）
- 使用 QThread 实现进程信息的异步刷新，避免界面卡顿
- 使用 `QTableWidget` 展示数据，搭配 `QMenu` 实现右键操作

## 📂 项目结构说明

| 文件/目录           | 说明                                  |
|--------------------|---------------------------------------|
| `main.cpp`         | 程序入口                              |
| `mainwindow.*`     | 主界面逻辑，信号槽定义                 |
| `processfetcher.*` | 子线程类，定期获取进程列表             |
| `processutils.*`   | 封装 Windows API，如终止进程、提取图标 |
| `style.qss`        | 界面美化 QSS 样式表                    |
| `.gitignore`       | 忽略 Qt 编译生成的中间文件             |

## 💻 项目运行方法

1. 使用 Qt Creator 打开 `.pro` 项目文件
2. 编译并运行（建议 Release 模式）
3. 程序会自动实时加载进程列表，无需手动刷新

## ✅ 运行要求

- Windows 10 或以上系统
- 已安装 Qt 5.6 及以上版本，推荐使用 Qt Creator 4.0.3
- 编译器：MinGW（或 MSVC）

## 🧠 开发目的与收获

通过本项目深入理解了：
- Windows 系统 API 的使用
- Qt GUI 编程与线程机制
- 项目结构设计、跨模块调用
- 使用 Git 进行版本控制与推送到 GitHub

## 📜 版权说明

本项目仅用于学习用途，欢迎参考和扩展。如有改进建议欢迎提 Issue 或 Pull Request。

> 作者：@Gently123  
> 项目地址：[https://github.com/Gently123/QTapp](https://github.com/Gently123/QTapp)


