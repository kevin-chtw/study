#pragma once

class CTKJunQiPKService : public CTKDllServer
{
public:
    CTKJunQiPKService(void);
    virtual ~CTKJunQiPKService(void);

    //实现接口
public:
    BOOL OnInitialUpdate() override;
    BOOL OnRegisterTKObject() override;
    CTKGame* OnCreateGame() override;
    CTKGamePlayer* OnCreateTKGamePlayer() override;

    const std::string& GetProductVersion();

private:
    std::string _initProductVersion();

private:
    std::string m_szProductVersion;
};
