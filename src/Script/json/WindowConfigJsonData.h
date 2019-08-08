/*
 * ======================= WindowConfigJsonData.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.08.08
 *                                        修改 -- 
 * ----------------------------------------------------------
 * 从 json文件中读取的 有关 windowConfig 的固定数据
 */
#ifndef TPR_WINDOW_CONFIG_JSON_DATA_H
#define TPR_WINDOW_CONFIG_JSON_DATA_H


class WindowConfigJsonData{
public:
    int windowPixW {};
    int windowPixH {};
    bool isFullScreen {};
    //...
};

WindowConfigJsonData parse_from_windowConfigJsonFile();


#endif 


