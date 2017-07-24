
#ifndef __T3D_FREE_TYPE_ATLAS_H__
#define __T3D_FREE_TYPE_ATLAS_H__


#include "Resource/T3DFontFreeType.h"


namespace Tiny3D
{
    class FreeTypeAtlas : public Singleton<FreeTypeAtlas>
    {
        friend class FontManager;

    public:
        virtual ~FreeTypeAtlas();

        bool updateContent(FontFreeTypePtr font, const String &text, MaterialPtr &material, Font::CharSet &set);

    protected:
        // �洢�����ַ���Ӧ����Ϣ
        typedef std::map<int32_t, Font::CharPtr>    CharMap;
        typedef CharMap::iterator                   CharMapItr;
        typedef CharMap::const_iterator             CharMapConstItr;
        typedef CharMap::value_type                 CharMapValue;

        struct Block : public Object
        {
            Block()
                : size(0)
            {}

            ObjectPtr       face;       /// �����������������
            size_t          size;       /// �����С
            Rect            area;       /// block������
            Point           offset;     /// �ַ���ʼλ��
            CharMap         charmap;    /// �ַ���
        };

        T3D_DECLARE_SMART_PTR(Block);


        // �洢������ͬ������ۺ���ͬ�����С��block��Ϣ��һ��Ԫ����һ��block
        typedef std::list<BlockPtr>             BlockList;
        typedef BlockList::iterator             BlockListItr;
        typedef BlockList::const_iterator       BlockListConstItr;
        typedef BlockList::value_type           BlockListValue;

        // �洢������ͬ������۲�ͬ��С��block��Ϣ����ͬ�����С��ͨ��һ��list��������
        typedef std::map<size_t, BlockList>     BlockMap;
        typedef BlockMap::iterator              BlockMapItr;
        typedef BlockMap::const_iterator        BlockMapConstItr;
        typedef BlockMap::value_type            BlockMapValue;

        struct Face : public Object
        {
            Face()
            {}

            String          name;       /// �����������
            MaterialPtr     material;   /// ��Ӧ������۵Ĳ���
            BlockMap        blockmap;   /// blockmap
        };

        T3D_DECLARE_SMART_PTR(Face);

        // ��ͬ��������ۣ���һ��������������һ��Ԫ����һ��������۶�Ӧ��blockmap
        typedef std::list<FacePtr>              FaceList;
        typedef FaceList::iterator              FaceListItr;
        typedef FaceList::const_iterator        FaceListConstItr;
        typedef FaceList::value_type            FaceListValue;

        // �洢���в�ͬ�������
        typedef std::map<String, FaceList>      FaceMap;
        typedef FaceMap::iterator               FaceMapItr;
        typedef FaceMap::const_iterator         FaceMapConstItr;
        typedef FaceMap::value_type             FaceMapValue;

        FreeTypeAtlas();

        /**
         * @brief ����charmap
         * @param [in] code : �ַ�����
         * @param [out] block : ���ص��������
         * @param [out] ch : ���ص��ַ���Ϣ����
         * @return ���ҵ�����true�����򷵻�false
         */
        bool lookupCharMap(FontFreeTypePtr font, int32_t code, Font::CharPtr &ch);

        /**
         * @brief �����ַ���charmap
         * @param [in] code : �ַ�����
         * @param [in] block : �������
         * @param [out] ch : ���ص��ַ���Ϣ����
         * @return ����ɹ�����true�����򷵻�false
         */
        bool insertCharMap(FontFreeTypePtr font, int32_t code, Font::CharPtr &ch);

        /**
         * @brief �����ʺϵ�����
         * @param [out] block : ���ص��������
         * @return ���ҵ��ʺϵķ���true�����򷵻�false
         */
        bool lookupBlock(FontFreeTypePtr font, BlockPtr &block);

        /**
         * @brief ����һ������
         * @param [out] block : ���ص��������
         * @return ����ɹ�����true�����򷵻�false
         */
        bool insertBlock(FontFreeTypePtr font, BlockPtr &block);

        bool insertBlockWithAppend(FontFreeTypePtr font, BlockPtr &block);

        bool insertBlockWithLRU(FontFreeTypePtr font, BlockPtr &block);

        bool loadMaterial(const String &name, FontFreeTypePtr font, MaterialPtr &material);

        bool createFace(MaterialPtr material, const String &fontName, FacePtr &face);

        bool createBlock(size_t fontSize, const Rect &area, const Point &offset, FacePtr face, BlockPtr &block);

    protected:
        FaceMap     mFaces;
        CharMap     mCharmap;           /// �洢�����ַ����������ڲ���
    };
}


#endif  /*__T3D_FREE_TYPE_ATLAS_H__*/