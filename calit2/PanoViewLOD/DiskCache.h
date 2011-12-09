#ifndef PANOVIEWLOD_DISK_CACHE_H
#define PANOVIEWLOD_DISK_CACHE_H

#include <OpenThreads/Thread>
#include <OpenThreads/Mutex>

#include <map>
#include <string>
#include <list>
#include <vector>

#include <tiffio.h>

struct sph_task;
class sph_cache;

enum JobType
{
    READ_THREAD = 0,
    COPY_THREAD
};

struct CopyJobInfo
{
    unsigned char * data;
    unsigned int size;
    unsigned int valid;
    OpenThreads::Mutex lock;
};

struct DiskCacheEntry
{
    CopyJobInfo * cji;
    int timestamp;
    int f;
    int i;
};

class JobThread: public OpenThreads::Thread
{
    public:
        JobThread(JobType jt, std::vector<std::list<std::pair<sph_task*, CopyJobInfo*> > > * readlist, std::vector<std::vector<std::list<std::pair<sph_task*, CopyJobInfo*> > > > * copylist, std::list<JobThread*> * freeThreadList, std::map<int, std::map<int,DiskCacheEntry*> > * cacheMap, std::map<sph_cache*,int> * cacheIndexMap);
        virtual ~JobThread();

        void run();
        void quit();

    protected:
        void read();
        void copy();

        void readData(sph_task * task, CopyJobInfo* cji, TIFF * T, uint32 w, uint32 h, uint16 c, uint16 b);

        OpenThreads::Mutex _quitLock;
        bool _quit;

        int _readIndex;
        int _copyCacheNum;
        int _copyFileNum;

        JobType _jt;
        std::vector<std::list<std::pair<sph_task*, CopyJobInfo*> > > * _readList;
        std::vector<std::vector<std::list<std::pair<sph_task*, CopyJobInfo*> > > > * _copyList;
        std::map<int, std::map<int,DiskCacheEntry*> > * _cacheMap;
        std::map<sph_cache*,int> * _cacheIndexMap;

        std::list<JobThread*> * _freeThreadList;
};

class DiskCache
{
    public:
        DiskCache(int pages);
        virtual ~DiskCache();

        int add_file(const std::string& name);
        void add_task(sph_task * task);

    protected:
        int _pages;
        int _numPages;

        void eject();

        OpenThreads::Mutex _fileAddLock;
        std::map<std::string, int> _fileIDMap;
        int _nextID;

        std::vector<JobThread*> _readThreads;
        std::vector<JobThread*> _copyThreads;

        std::list<JobThread*> _freeThreadList;

        std::vector<std::list<std::pair<sph_task*, CopyJobInfo*> > > _readList;
        std::vector<std::vector<std::list<std::pair<sph_task*, CopyJobInfo*> > > > _copyList;

        std::map<int, std::map<int,DiskCacheEntry*> > _cacheMap;

        std::map<sph_cache*,int> _cacheIndexMap;
};

#endif
