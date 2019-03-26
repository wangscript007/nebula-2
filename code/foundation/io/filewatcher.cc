//------------------------------------------------------------------------------
//  filewatcher.cc
//  (C) 2019 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "foundation/stdneb.h"
#include "io/filewatcher.h"

namespace IO
{
__ImplementClass(IO::FileWatcher, 'FIWT', Core::RefCounted);
__ImplementSingleton(IO::FileWatcher);

using namespace IO;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
FileWatcher::FileWatcher()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
FileWatcher::~FileWatcher()
{
    n_assert(this->watchers.IsEmpty());
    __DestructSingleton;
}


//------------------------------------------------------------------------------
/**
*/
void
FileWatcher::Update()
{
    for (auto& w : this->watchers)
    {        
        FileWatcherImpl::Update(w.Value());        
    }
}

//------------------------------------------------------------------------------
/**
*/
void
FileWatcher::Watch(Util::String const & folder, bool recursive, WatchDelegate const & callback)
{
    n_assert(!this->watchers.Contains(folder));    
    this->watchers.Add(folder, { callback,folder});
    EventHandlerData& eventData = this->watchers[folder];
    FileWatcherImpl::CreateWatcher(eventData);        
}

//------------------------------------------------------------------------------
/**
*/
void
FileWatcher::Unwatch(Util::String const & folder)
{    
    n_assert(this->watchers.Contains(folder));
    auto& eventData = this->watchers[folder];
    FileWatcherImpl::DestroyWatcher(eventData);
    this->watchers.Erase(folder);
}

}