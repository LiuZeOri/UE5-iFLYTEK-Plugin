# UE5-iFLYTEK-Plugin
UE5 虚幻引擎5 整合科大讯飞语音识别、合成等 插件

## 关于作者
邮箱：1564355910@qq.com\
b站：https://space.bilibili.com/44520318?spm_id_from=333.1007.0.0 \
个人知识库：https://www.yuque.com/u28988421/ad9c7i?# 《UE知识宝库》\
私信b站回复最快

## 开发功能
实时语音转写\
在线语音合成\
实时语音听写\
文本合规\
星火大模型（历史消息回复待开发）\
情感分析（待开发）\
离线语音听写（待开发）\
语音唤醒（待开发）\
离线语音合成（待开发）

支持Windows、Android（待开发）、IOS（待开发）

## 使用方法
1.将Plugins中的IFlytekVoice插件拷贝到自己项目的Plugins文件夹中\
2.Config/IFlytekConfig.ini为配置文件，在此填入科大讯飞申请的appID和apiKey\
3.第一次使用插件时，只需蓝图运行一下插件蓝图节点，即可自动在Config文件夹中生成配置文件。也可手动新建配置文件\
4.打包后，需要新建 打包文件夹/项目名称/Config/IFlytekConfig.ini，配置文件与工程文件的配置文件一致\
5.目前项目的版本为UE5.03，更高版本需从sln打开工程，重新编译插件即可\
6.插件的使用方法在提供的示例工程文件的Test和TestSparkDesk关卡中，供参考

## 待解决问题
1、星火大模型返回的结果怎么返回一句就合成一次音频并播放

