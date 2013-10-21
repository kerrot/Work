#include "ComicUIObject.h"
#include "ButtonObject.h"
#include "dirent.h"
#include "GameObjectFactory.h"

ComicUIObject::ComicUIObject(GameObject* a_ui, GameObject* a_shadow)
:
MainWindowInterface(this, a_ui, a_shadow)
,m_index(0)
,m_oriWidth(0)
,m_oriHeight(0)
,m_scale(1)
{

}

ComicUIObject::~ComicUIObject()
{
    
}

void ComicUIObject::Init()
{
    DIR *dp;
    struct dirent *dirp;
    if(dp = opendir("Comic"))
    {
        while((dirp = readdir(dp)) != NULL)
        {
            std::string file = "Comic/";
            file += dirp->d_name;
            if (file == "Comic/." || file == "Comic/..")
            {
                continue;
            }

            m_comicList.push_back(file);
        }
        closedir(dp);    
    }

    m_closeButton->SetPosition(0.5, SUBWINDOW_DISTANCE, 0.5);
    float w = 10, h = 10;
    m_closeButton->Resize(w, h);
    m_closeButton->SetScale(PMVector(0.1, 1, 0.1));
    m_closeButton->SetButtonDisplay(TEXTURE_CLOSE_NORMAL, TEXTURE_CLOSE_HOVER, TEXTURE_CLOSE_PRESS, TEXTURE_CLOSE_DISABLE);
    m_closeButton->ReleaseEvent.AddCallBack(this, &ComicUIObject::Close);
    m_subWindows.push_back(m_closeButton);

    m_upButton->SetPosition(-0.5, SUBWINDOW_DISTANCE, 0);
    w = 10; h = 10;
    m_upButton->Resize(w, h);
    m_upButton->SetScale(PMVector(0.1, 1, 0.1));
    m_upButton->SetButtonDisplay(TEXTURE_LAST_NORMAL, TEXTURE_LAST_HOVER, TEXTURE_LAST_PRESS, TEXTURE_LAST_DISABLE);
    m_upButton->ReleaseEvent.AddCallBack(this, &ComicUIObject::PageUp);
    m_subWindows.push_back(m_upButton);

    m_downButton->SetPosition(0.5, SUBWINDOW_DISTANCE, 0);
    w = 10; h = 10;
    m_downButton->Resize(w, h);
    m_downButton->SetScale(PMVector(0.1, 1, 0.1));
    m_downButton->SetButtonDisplay(TEXTURE_NEXT_NORMAL, TEXTURE_NEXT_HOVER, TEXTURE_NEXT_PRESS, TEXTURE_NEXT_DISABLE);
    m_downButton->ReleaseEvent.AddCallBack(this, &ComicUIObject::PageDown);
    m_subWindows.push_back(m_downButton);

    UpdatePage();
}

void ComicUIObject::Close()
{
    SetVisible(false);
    SetEnabled(false);
}

void ComicUIObject::PageUp()
{
    if (m_index - 1 >= 0)
    {
        --m_index;
        UpdatePage();
    }
}

void ComicUIObject::PageDown()
{
    if (m_index + 1 < m_comicList.size())
    {
        ++m_index;
        UpdatePage();
    }
}

void ComicUIObject::UpdatePage()
{
    if (m_index >= 0 && m_index < m_comicList.size())
    {
        std::pair<UInt32, UInt32> textureSize = sGameObjectFactory.LoadTexture(this, m_comicList[m_index]);
        InitSize(textureSize.first / 10, textureSize.second / 10);

        m_upButton->SetEnabled(m_index > 0);
        m_downButton->SetEnabled(m_index < m_comicList.size() - 1);
    }
}

void ComicUIObject::InitSize( UInt32 a_width, UInt32 a_height )
{
    m_oriWidth = a_width;
    m_oriHeight = a_height;
    float w = m_oriWidth * m_scale, h = m_oriHeight * m_scale;
    Resize(w, h);
}

void ComicUIObject::Resize( float &a_width, float &a_height )
{
    m_scale = a_width / m_oriWidth;
    a_height = m_oriHeight * m_scale;

    m_width = a_width;
    m_height = a_height;

    m_object->SetScale(PMVector(m_width, 0, m_height));

    float w = 10.0f / m_width, h = 10.0f / m_height;
    m_closeButton->SetScale(PMVector(w, 1, h));
    m_upButton->SetScale(PMVector(w, 1, h));
    m_downButton->SetScale(PMVector(w, 1, h));
}
