#ifndef __C_SCENE_MANAGER_DECORATOR_H_INCLUDED__
#define __C_SCENE_MANAGER_DECORATOR_H_INCLUDED__

#include <ISceneManager.h>

namespace irr
{

namespace io
{
    class IFileSystem;
}

namespace gui
{
    class IGUIEnvironment;
}

namespace video
{
    class IVideoDriver;
}

namespace scene
{

    class IAnimatedMesh;
    class IMeshCache;
    class IVolumeLightSceneNode;
    class IMeshSceneNode;
    class IAnimatedMeshSceneNode;
    class IMeshSceneNode;

    class CSceneManagerDecorator : public ISceneManager
    {
    public:

        CSceneManagerDecorator(ISceneManager* smgr):SceneManager(smgr) { SceneManager->grab(); }

        ~CSceneManagerDecorator() { SceneManager->drop(); }

        virtual IAnimatedMesh* getMesh(const io::path& filename) { return SceneManager->getMesh(filename); }

        virtual IAnimatedMesh* getMesh(io::IReadFile* file) { return SceneManager->getMesh(file); }

        virtual IMeshCache* getMeshCache() { return SceneManager->getMeshCache(); }

        virtual video::IVideoDriver* getVideoDriver() { return SceneManager->getVideoDriver(); }

        virtual gui::IGUIEnvironment* getGUIEnvironment() { return SceneManager->getGUIEnvironment(); } 

        virtual io::IFileSystem* getFileSystem() { return SceneManager->getFileSystem(); }

        virtual IVolumeLightSceneNode* addVolumeLightSceneNode(ISceneNode* parent=0, s32 id=-1,
            const u32 subdivU = 32, const u32 subdivV = 32,
            const video::SColor foot = video::SColor(51, 0, 230, 180),
            const video::SColor tail = video::SColor(0, 0, 0, 0),
            const core::vector3df& position = core::vector3df(0,0,0),
            const core::vector3df& rotation = core::vector3df(0,0,0),
            const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f))
        {
            return SceneManager->addVolumeLightSceneNode(
                parent,
                id,
                subdivU,
                subdivV,
                foot,
                tail,
                position,
                rotation,
                scale
            );
        }

        virtual IMeshSceneNode* addCubeSceneNode(f32 size=10.0f, ISceneNode* parent=0, s32 id=-1,
            const core::vector3df& position = core::vector3df(0,0,0),
            const core::vector3df& rotation = core::vector3df(0,0,0),
            const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f))
        {
            return SceneManager->addCubeSceneNode(
                size,
                parent,
                id,
                position,
                rotation,
                scale
            );
        }

        virtual IMeshSceneNode* addSphereSceneNode(f32 radius=5.0f, s32 polyCount=16,
                ISceneNode* parent=0, s32 id=-1,
                const core::vector3df& position = core::vector3df(0,0,0),
                const core::vector3df& rotation = core::vector3df(0,0,0),
                const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f))
        {
            return SceneManager->addSphereSceneNode(
                radius,
                polyCount,
                parent,
                id,
                position,
                rotation,
                scale
            );
        }

        virtual IAnimatedMeshSceneNode* addAnimatedMeshSceneNode(IAnimatedMesh* mesh,
                ISceneNode* parent=0, s32 id=-1,
                const core::vector3df& position = core::vector3df(0,0,0),
                const core::vector3df& rotation = core::vector3df(0,0,0),
                const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f),
                bool alsoAddIfMeshPointerZero=false)
        {
            return SceneManager->addAnimatedMeshSceneNode(
                mesh,
                parent,
                id,
                position,
                rotation,
                scale,
                alsoAddIfMeshPointerZero
            );
        }

        virtual IMeshSceneNode* addMeshSceneNode(IMesh* mesh, ISceneNode* parent=0, s32 id=-1,
            const core::vector3df& position = core::vector3df(0,0,0),
            const core::vector3df& rotation = core::vector3df(0,0,0),
            const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f),
            bool alsoAddIfMeshPointerZero=false)
        {
            return SceneManager->addMeshSceneNode(
                mesh,
                parent,
                id,
                position,
                rotation,
                scale,
                alsoAddIfMeshPointerZero
            ); 
        }

        virtual ISceneNode* addWaterSurfaceSceneNode(IMesh* mesh,
            f32 waveHeight=2.0f, f32 waveSpeed=300.0f, f32 waveLength=10.0f,
            ISceneNode* parent=0, s32 id=-1,
            const core::vector3df& position = core::vector3df(0,0,0),
            const core::vector3df& rotation = core::vector3df(0,0,0),
            const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f))
        {
            return SceneManager->addWaterSurfaceSceneNode(
                mesh,
                waveHeight,
                waveSpeed,
                waveLength,
                parent,
                id,
                position,
                rotation,
                scale
            );
        }

        virtual IMeshSceneNode* addOctreeSceneNode(IAnimatedMesh* mesh, ISceneNode* parent=0,
            s32 id=-1, s32 minimalPolysPerNode=512, bool alsoAddIfMeshPointerZero=false)
        {
            return SceneManager->addOctreeSceneNode(
                mesh,
                parent,
                id,
                minimalPolysPerNode,
                alsoAddIfMeshPointerZero
            );
        }

        virtual IMeshSceneNode* addOctreeSceneNode(IMesh* mesh, ISceneNode* parent=0,
            s32 id=-1, s32 minimalPolysPerNode=256, bool alsoAddIfMeshPointerZero=false)
        {
            return SceneManager->addOctreeSceneNode(
                mesh,
                parent,
                id,
                minimalPolysPerNode,
                alsoAddIfMeshPointerZero
            );
        }

        virtual ICameraSceneNode* addCameraSceneNode(ISceneNode* parent = 0,
            const core::vector3df& position = core::vector3df(0,0,0),
            const core::vector3df& lookat = core::vector3df(0,0,100),
            s32 id=-1, bool makeActive=true)
        {
            return SceneManager->addCameraSceneNode(
                parent,
                position,
                lookat,
                id,
                makeActive
            );
        }

        virtual ICameraSceneNode* addCameraSceneNodeMaya(ISceneNode* parent=0,
            f32 rotateSpeed=-1500.f, f32 zoomSpeed=200.f,
            f32 translationSpeed=1500.f, s32 id=-1, f32 distance=70.f,
            bool makeActive=true)
        {
            return SceneManager->addCameraSceneNodeMaya(
                parent,
                rotateSpeed,
                zoomSpeed,
                translationSpeed,
                id,
                distance,
                makeActive
            );
        }

        virtual ICameraSceneNode* addCameraSceneNodeFPS(ISceneNode* parent = 0,
            f32 rotateSpeed = 100.0f, f32 moveSpeed = 0.5f, s32 id=-1,
            SKeyMap* keyMapArray=0, s32 keyMapSize=0, bool noVerticalMovement=false,
            f32 jumpSpeed = 0.f, bool invertMouse=false,
            bool makeActive=true)
        {
            return SceneManager->addCameraSceneNodeFPS(
                parent,
                rotateSpeed,
                moveSpeed,
                id,
                keyMapArray,
                keyMapSize,
                noVerticalMovement,
                jumpSpeed,
                invertMouse,
                makeActive
            );
        }

        virtual ILightSceneNode* addLightSceneNode(ISceneNode* parent = 0,
            const core::vector3df& position = core::vector3df(0,0,0),
            video::SColorf color = video::SColorf(1.0f, 1.0f, 1.0f),
            f32 radius=100.0f, s32 id=-1)
        {
            return SceneManager->addLightSceneNode(
                parent,
                position,
                color,
                radius,
                id
            );
        }

        virtual IBillboardSceneNode* addBillboardSceneNode(ISceneNode* parent = 0,
            const core::dimension2d<f32>& size = core::dimension2d<f32>(10.0f, 10.0f),
            const core::vector3df& position = core::vector3df(0,0,0), s32 id=-1,
            video::SColor colorTop = 0xFFFFFFFF, video::SColor colorBottom = 0xFFFFFFFF)
        {
            return SceneManager->addBillboardSceneNode(
                parent,
                size,
                position,
                id,
                colorTop,
                colorBottom
            );
        }

        virtual ISceneNode* addSkyBoxSceneNode(video::ITexture* top, video::ITexture* bottom,
            video::ITexture* left, video::ITexture* right, video::ITexture* front,
            video::ITexture* back, ISceneNode* parent = 0, s32 id=-1)
        {
            return SceneManager->addSkyBoxSceneNode(
                top,
                bottom,
                left,
                right,
                front,
                back,
                parent,
                id
            );
        }

        virtual ISceneNode* addSkyDomeSceneNode(video::ITexture* texture,
            u32 horiRes=16, u32 vertRes=8,
            f32 texturePercentage=0.9, f32 spherePercentage=2.0,f32 radius = 1000.f,
            ISceneNode* parent=0, s32 id=-1)
        {
            return SceneManager->addSkyDomeSceneNode(
                texture,
                horiRes,
                vertRes,
                texturePercentage,
                spherePercentage,
                radius,
                parent,
                id
            );
        }

        virtual IParticleSystemSceneNode* addParticleSystemSceneNode(
            bool withDefaultEmitter=true, ISceneNode* parent=0, s32 id=-1,
            const core::vector3df& position = core::vector3df(0,0,0),
            const core::vector3df& rotation = core::vector3df(0,0,0),
            const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f))
        {
            return SceneManager->addParticleSystemSceneNode(
                withDefaultEmitter,
                parent,
                id,
                position,
                rotation,
                scale
            );
        }

        virtual ITerrainSceneNode* addTerrainSceneNode(
            const io::path& heightMapFileName,
            ISceneNode* parent=0, s32 id=-1,
            const core::vector3df& position = core::vector3df(0.0f,0.0f,0.0f),
            const core::vector3df& rotation = core::vector3df(0.0f,0.0f,0.0f),
            const core::vector3df& scale = core::vector3df(1.0f,1.0f,1.0f),
            video::SColor vertexColor = video::SColor(255,255,255,255),
            s32 maxLOD=5, E_TERRAIN_PATCH_SIZE patchSize=ETPS_17, s32 smoothFactor=0,
            bool addAlsoIfHeightmapEmpty = false)
        {
            return SceneManager->addTerrainSceneNode(
                heightMapFileName,
                parent,
                id,
                position,
                rotation,
                scale,
                vertexColor,
                maxLOD,
                patchSize,
                smoothFactor,
                addAlsoIfHeightmapEmpty
            );
        }

        virtual ITerrainSceneNode* addTerrainSceneNode(
            io::IReadFile* heightMapFile,
            ISceneNode* parent=0, s32 id=-1,
            const core::vector3df& position = core::vector3df(0.0f,0.0f,0.0f),
            const core::vector3df& rotation = core::vector3df(0.0f,0.0f,0.0f),
            const core::vector3df& scale = core::vector3df(1.0f,1.0f,1.0f),
            video::SColor vertexColor = video::SColor(255,255,255,255),
            s32 maxLOD=5, E_TERRAIN_PATCH_SIZE patchSize=ETPS_17, s32 smoothFactor=0,
            bool addAlsoIfHeightmapEmpty = false)
        {
            return SceneManager->addTerrainSceneNode(
                heightMapFile,
                parent,
                id,
                position,
                rotation,
                scale,
                vertexColor,
                maxLOD,
                patchSize,
                smoothFactor,
                addAlsoIfHeightmapEmpty
            );
        }

        virtual IMeshSceneNode* addQuake3SceneNode(
            const IMeshBuffer* meshBuffer, const quake3::IShader * shader,
            ISceneNode* parent=0, s32 id=-1)
        {
            return SceneManager->addQuake3SceneNode(
                meshBuffer,
                shader,
                parent,
                id
            );
        }

        virtual ISceneNode* addEmptySceneNode(ISceneNode* parent=0, s32 id=-1)
        {
            return SceneManager->addEmptySceneNode(parent, id);
        }

        virtual IDummyTransformationSceneNode* addDummyTransformationSceneNode(
            ISceneNode* parent=0, s32 id=-1)
        {
            return SceneManager->addDummyTransformationSceneNode(parent, id);
        }

        virtual ITextSceneNode* addTextSceneNode(gui::IGUIFont* font, const wchar_t* text,
            video::SColor color=video::SColor(100,255,255,255),
            ISceneNode* parent = 0, const core::vector3df& position = core::vector3df(0,0,0),
            s32 id=-1)
        {
            return SceneManager->addTextSceneNode(
                font,
                text,
                color,
                parent,
                position,
                id
            );
        }

        virtual IBillboardTextSceneNode* addBillboardTextSceneNode(
            gui::IGUIFont* font, const wchar_t* text,
            ISceneNode* parent = 0,
            const core::dimension2d<f32>& size = core::dimension2d<f32>(10.0f, 10.0f),
            const core::vector3df& position = core::vector3df(0,0,0), s32 id=-1,
            video::SColor colorTop = 0xFFFFFFFF, video::SColor colorBottom = 0xFFFFFFFF)
        {
            return SceneManager->addBillboardTextSceneNode(
                font,
                text,
                parent,
                size,
                position,
                id,
                colorTop,
                colorBottom
            );
        }

        virtual IAnimatedMesh* addHillPlaneMesh(const io::path& name,
            const core::dimension2d<f32>& tileSize, const core::dimension2d<u32>& tileCount,
            video::SMaterial* material = 0, f32 hillHeight = 0.0f,
            const core::dimension2d<f32>& countHills = core::dimension2d<f32>(0.0f, 0.0f),
            const core::dimension2d<f32>& textureRepeatCount = core::dimension2d<f32>(1.0f, 1.0f))
        {
            return SceneManager->addHillPlaneMesh(
                name,
                tileSize,
                tileCount,
                material,
                hillHeight,
                countHills,
                textureRepeatCount
            );
        }

        virtual IAnimatedMesh* addTerrainMesh(const io::path& meshname,
            video::IImage* texture, video::IImage* heightmap,
            const core::dimension2d<f32>& stretchSize = core::dimension2d<f32>(10.0f,10.0f),
            f32 maxHeight=200.0f,
            const core::dimension2d<u32>& defaultVertexBlockSize = core::dimension2d<u32>(64,64))
        {
            return SceneManager->addTerrainMesh(
                meshname,
                texture,
                heightmap,
                stretchSize,
                maxHeight,
                defaultVertexBlockSize
            );
        }

        virtual IAnimatedMesh* addArrowMesh(const io::path& name,
                video::SColor vtxColorCylinder=0xFFFFFFFF,
                video::SColor vtxColorCone=0xFFFFFFFF,
                u32 tesselationCylinder=4, u32 tesselationCone=8,
                f32 height=1.f, f32 cylinderHeight=0.6f,
                f32 widthCylinder=0.05f, f32 widthCone=0.3f)
        {
            return SceneManager->addArrowMesh(
                name,
                vtxColorCylinder,
                vtxColorCone,
                tesselationCylinder,
                tesselationCone,
                height,
                cylinderHeight,
                widthCylinder,
                widthCone
            );
        }

        virtual IAnimatedMesh* addSphereMesh(const io::path& name,
                f32 radius=5.f, u32 polyCountX = 16,
                u32 polyCountY = 16)
        {
            return SceneManager->addSphereMesh(
                name,
                radius,
                polyCountX,
                polyCountY
            );
        }

        virtual IAnimatedMesh* addVolumeLightMesh(const io::path& name,
                const u32 SubdivideU = 32, const u32 SubdivideV = 32,
                const video::SColor FootColor = video::SColor(51, 0, 230, 180),
                const video::SColor TailColor = video::SColor(0, 0, 0, 0))
        {
            return SceneManager->addVolumeLightMesh(
                name,
                SubdivideU,
                SubdivideV,
                FootColor,
                TailColor
            );
        }

        virtual ISceneNode* getRootSceneNode() { return SceneManager->getRootSceneNode(); }

        virtual ISceneNode* getSceneNodeFromId(s32 id, ISceneNode* start=0)
        {
            return SceneManager->getSceneNodeFromId(id, start);
        }

        virtual ISceneNode* getSceneNodeFromName(const c8* name, ISceneNode* start=0)
        {
            return SceneManager->getSceneNodeFromName(name, start);
        }

        virtual ISceneNode* getSceneNodeFromType(scene::ESCENE_NODE_TYPE type, ISceneNode* start=0)
        {
            return SceneManager->getSceneNodeFromType(type, start);
        }

        virtual void getSceneNodesFromType(ESCENE_NODE_TYPE type,
                core::array<scene::ISceneNode*>& outNodes,
                ISceneNode* start=0)
        {
            return SceneManager->getSceneNodesFromType(type, outNodes, start);
        }

        virtual ICameraSceneNode* getActiveCamera() const { return SceneManager->getActiveCamera(); }

        virtual void setActiveCamera(ICameraSceneNode* camera)
        {
            return SceneManager->setActiveCamera(camera);
        }

        virtual void setShadowColor(video::SColor color = video::SColor(150,0,0,0))
        {
            return SceneManager->setShadowColor(color);
        }

        virtual video::SColor getShadowColor() const { return SceneManager->getShadowColor(); }

        virtual u32 registerNodeForRendering(ISceneNode* node,
            E_SCENE_NODE_RENDER_PASS pass = ESNRP_AUTOMATIC)
        {
            return SceneManager->registerNodeForRendering(node, pass);
        }

        virtual void drawAll() { return SceneManager->drawAll(); }

        virtual ISceneNodeAnimator* createRotationAnimator(const core::vector3df& rotationSpeed)
        {
            return SceneManager->createRotationAnimator(rotationSpeed);
        }

        virtual ISceneNodeAnimator* createFlyCircleAnimator(
                const core::vector3df& center=core::vector3df(0.f,0.f,0.f),
                f32 radius=100.f, f32 speed=0.001f,
                const core::vector3df& direction=core::vector3df(0.f, 1.f, 0.f),
                f32 startPosition = 0.f,
                f32 radiusEllipsoid = 0.f)
        {
            return SceneManager->createFlyCircleAnimator(
                center,
                radius,
                speed,
                direction,
                startPosition,
                radiusEllipsoid
            );
        }

        virtual ISceneNodeAnimator* createFlyStraightAnimator(const core::vector3df& startPoint,
            const core::vector3df& endPoint, u32 timeForWay, bool loop=false, bool pingpong = false)
        {
            return SceneManager->createFlyStraightAnimator(
                startPoint,
                endPoint,
                timeForWay,
                loop,
                pingpong
            );
        }

        virtual ISceneNodeAnimator* createTextureAnimator(const core::array<video::ITexture*>& textures,
            s32 timePerFrame, bool loop=true)
        {
            return SceneManager->createTextureAnimator(
                textures,
                timePerFrame,
                loop
            );
        }

        virtual ISceneNodeAnimator* createDeleteAnimator(u32 timeMs)
        {
            return SceneManager->createDeleteAnimator(timeMs);
        }

        virtual ISceneNodeAnimatorCollisionResponse* createCollisionResponseAnimator(
            ITriangleSelector* world, ISceneNode* sceneNode,
            const core::vector3df& ellipsoidRadius = core::vector3df(30,60,30),
            const core::vector3df& gravityPerSecond = core::vector3df(0,-10.0f,0),
            const core::vector3df& ellipsoidTranslation = core::vector3df(0,0,0),
            f32 slidingValue = 0.0005f)
        {
            return SceneManager->createCollisionResponseAnimator(
                world,
                sceneNode,
                ellipsoidRadius,
                gravityPerSecond,
                ellipsoidTranslation,
                slidingValue
            );
        }

        virtual ISceneNodeAnimator* createFollowSplineAnimator(s32 startTime,
            const core::array< core::vector3df >& points,
            f32 speed = 1.0f, f32 tightness = 0.5f, bool loop=true, bool pingpong=false)
        {
            return SceneManager->createFollowSplineAnimator(
                startTime,
                points,
                speed,
                tightness,
                loop,
                pingpong
            );
        }

        virtual ITriangleSelector* createTriangleSelector(IMesh* mesh, ISceneNode* node)
        {
            return SceneManager->createTriangleSelector(mesh, node);
        }

        virtual ITriangleSelector* createTriangleSelector(IAnimatedMeshSceneNode* node)
        {
            return SceneManager->createTriangleSelector(node);
        }

        virtual ITriangleSelector* createTriangleSelectorFromBoundingBox(ISceneNode* node)
        {
            return SceneManager->createTriangleSelectorFromBoundingBox(node);
        }

        virtual ITriangleSelector* createOctreeTriangleSelector(IMesh* mesh,
            ISceneNode* node, s32 minimalPolysPerNode=32)
        {
            return SceneManager->createOctreeTriangleSelector(mesh, node, minimalPolysPerNode);
        }

        virtual IMetaTriangleSelector* createMetaTriangleSelector()
        {
            return SceneManager->createMetaTriangleSelector();
        }

        virtual ITriangleSelector* createTerrainTriangleSelector(
            ITerrainSceneNode* node, s32 LOD=0)
        {
            return SceneManager->createTerrainTriangleSelector(node, LOD);
        }

        virtual void addExternalMeshLoader(IMeshLoader* externalLoader)
        {
            return SceneManager->addExternalMeshLoader(externalLoader);
        }

        virtual u32 getMeshLoaderCount() const { return SceneManager->getMeshLoaderCount(); }

        virtual IMeshLoader* getMeshLoader(u32 index) const
        {
            return SceneManager->getMeshLoader(index);
        }

        virtual void addExternalSceneLoader(ISceneLoader* externalLoader)
        {
            return SceneManager->addExternalSceneLoader(externalLoader);
        }

        virtual u32 getSceneLoaderCount() const { return SceneManager->getSceneLoaderCount(); }

        virtual ISceneLoader* getSceneLoader(u32 index) const
        {
            return SceneManager->getSceneLoader(index);
        }

        virtual ISceneCollisionManager* getSceneCollisionManager()
        {
            return SceneManager->getSceneCollisionManager();
        }

        virtual IMeshManipulator* getMeshManipulator()
        {
            return SceneManager->getMeshManipulator();
        }

        virtual void addToDeletionQueue(ISceneNode* node)
        {
            return SceneManager->addToDeletionQueue(node);
        }

        virtual bool postEventFromUser(const SEvent& event)
        {
            return SceneManager->postEventFromUser(event);
        }

        virtual void clear() { return SceneManager->clear(); }

        virtual io::IAttributes* getParameters() { return SceneManager->getParameters(); }

        virtual E_SCENE_NODE_RENDER_PASS getSceneNodeRenderPass() const
        {
            return SceneManager->getSceneNodeRenderPass();
        }

        virtual ISceneNodeFactory* getDefaultSceneNodeFactory()
        {
            return SceneManager->getDefaultSceneNodeFactory();
        }

        virtual void registerSceneNodeFactory(ISceneNodeFactory* factoryToAdd)
        {
            return SceneManager->registerSceneNodeFactory(factoryToAdd);
        }

        virtual u32 getRegisteredSceneNodeFactoryCount() const
        {
            return SceneManager->getRegisteredSceneNodeFactoryCount();
        }

        virtual ISceneNodeFactory* getSceneNodeFactory(u32 index)
        {
            return SceneManager->getSceneNodeFactory(index);
        }

        virtual ISceneNodeAnimatorFactory* getDefaultSceneNodeAnimatorFactory()
        {
            return SceneManager->getDefaultSceneNodeAnimatorFactory();
        }

        virtual void registerSceneNodeAnimatorFactory(ISceneNodeAnimatorFactory* factoryToAdd)
        {
            return SceneManager->registerSceneNodeAnimatorFactory(factoryToAdd);
        }

        virtual u32 getRegisteredSceneNodeAnimatorFactoryCount() const
        {
            return SceneManager->getRegisteredSceneNodeAnimatorFactoryCount();
        }

        virtual ISceneNodeAnimatorFactory* getSceneNodeAnimatorFactory(u32 index)
        {
            return SceneManager->getSceneNodeAnimatorFactory(index);
        }

        virtual const c8* getSceneNodeTypeName(ESCENE_NODE_TYPE type)
        {
            return SceneManager->getSceneNodeTypeName(type);
        }

        virtual const c8* getAnimatorTypeName(ESCENE_NODE_ANIMATOR_TYPE type)
        {
            return SceneManager->getAnimatorTypeName(type);
        }

        virtual ISceneNode* addSceneNode(const char* sceneNodeTypeName, ISceneNode* parent=0)
        {
            return SceneManager->addSceneNode(sceneNodeTypeName, parent);
        }

        virtual ISceneNodeAnimator* createSceneNodeAnimator(
            const char* typeName, ISceneNode* target=0)
        {
            return SceneManager->createSceneNodeAnimator(typeName, target);
        }

        virtual ISceneManager* createNewSceneManager(bool cloneContent=false)
        {
            return SceneManager->createNewSceneManager(cloneContent);
        }

        virtual bool saveScene(const io::path& filename,
            ISceneUserDataSerializer* userDataSerializer=0, ISceneNode* node=0)
        {
            return SceneManager->saveScene(filename, userDataSerializer, node);
        }

        virtual bool saveScene(io::IWriteFile* file,
            ISceneUserDataSerializer* userDataSerializer=0, ISceneNode* node=0)
        {
            return SceneManager->saveScene(file, userDataSerializer, node);
        }

        virtual bool saveScene(io::IXMLWriter* writer,
            const io::path& currentPath, ISceneUserDataSerializer* userDataSerializer=0,
            ISceneNode* node=0)
        {
            return SceneManager->saveScene(
                writer,
                currentPath,
                userDataSerializer,
                node
            );
        }

        virtual bool loadScene(const io::path& filename,
            ISceneUserDataSerializer* userDataSerializer=0, ISceneNode* rootNode=0)
        {
            return SceneManager->loadScene(
                filename,
                userDataSerializer,
                rootNode
            );
        }

        virtual bool loadScene(io::IReadFile* file,
            ISceneUserDataSerializer* userDataSerializer=0, ISceneNode* rootNode=0)
        {
            return SceneManager->loadScene(
                file,
                userDataSerializer,
                rootNode
            );
        }

        virtual IMeshWriter* createMeshWriter(EMESH_WRITER_TYPE type)
        {
            return SceneManager->createMeshWriter(type);
        }

        virtual ISkinnedMesh* createSkinnedMesh()
        {
            return SceneManager->createSkinnedMesh();
        }

        virtual void setAmbientLight(const video::SColorf &ambientColor)
        {
            return SceneManager->setAmbientLight(ambientColor);
        }

        virtual const video::SColorf& getAmbientLight() const
        {
            return SceneManager->getAmbientLight();
        }

        virtual void setLightManager(ILightManager* lightManager)
        {
            return SceneManager->setLightManager(lightManager);
        }

        virtual const IGeometryCreator* getGeometryCreator(void) const
        {
            return SceneManager->getGeometryCreator();
        }

        virtual bool isCulled(const ISceneNode* node) const
        {
            return SceneManager->isCulled(node);
        }

    protected:

        ISceneManager* SceneManager;

    };

}
}
#endif
