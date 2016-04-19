/***********************************************************************************************************
Copyright � Microsoft Open Technologies, Inc.
All Rights Reserved
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0

THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER
EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED WARRANTIES OR
CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.

See the Apache 2 License for the specific language governing permissions and limitations under the License.
***********************************************************************************************************/
#pragma once

#include "stdafx.h"

#ifndef _COMSMARTPTR_H_
#define _COMSMARTPTR_H_

#include <bemapiset.h>
#include <unknwn.h>
#include <assert.h>

template<class INTERFACE, const IID* piid = nullptr>
class ComSmartPtr
{
public:
    ComSmartPtr() { m_Ptr = nullptr; }

    // Apart from its original role this is used as conversion constructor for nullptr.
    ComSmartPtr(INTERFACE* lPtr)
    {
        m_Ptr = nullptr;

        if (lPtr != nullptr)
        {
            m_Ptr = lPtr;
            m_Ptr->AddRef();
        }
    }

    ComSmartPtr(const ComSmartPtr<INTERFACE, piid>& refComPtr)
    {
        m_Ptr = nullptr;

        // using static cast here since conversion operator is explicitly defined
        m_Ptr = static_cast<INTERFACE*>(refComPtr);

        if (m_Ptr)
        {
            m_Ptr->AddRef();
        }
    }

    ComSmartPtr(IUnknown* pIUnknown, IID iid)
    {
        m_Ptr = nullptr;

        if (pIUnknown != nullptr)
        {
            pIUnknown->QueryInterface(iid, reinterpret_cast<void**>(&m_Ptr));
        }
    }

    ~ComSmartPtr()
    {
        if (m_Ptr)
        {
            m_Ptr->Release();
            m_Ptr = nullptr;
        }
    }

public:
    operator INTERFACE*() const
    {
        // no assert() here since this will be called on e.g.:
        // if (nullptr == p_comSmartPtrInstance) { ... }
        // m_Ptr can be null and it'll be perfectly valid case.
        return m_Ptr;
    }

    INTERFACE& operator*()
    {
        assert(m_Ptr != nullptr);
        return *m_Ptr;
    }

    const INTERFACE& operator*() const
    {
        assert(m_Ptr != nullptr);
        return *m_Ptr;
    }

    INTERFACE** operator&()
    {
        //assert(m_Ptr != nullptr);
        return &m_Ptr;
    }

    INTERFACE* operator->() const
    {
        assert(m_Ptr != nullptr);
        return m_Ptr;
    }

    bool operator==(const INTERFACE& lPtr) const
    {
        assert(m_Ptr != nullptr);
        return m_Ptr == lPtr;
    }

    bool operator!=(const INTERFACE& lPtr) const
    {
        assert(m_Ptr != nullptr);
        return !(m_Ptr == lPtr);
    }

    INTERFACE* operator=(INTERFACE* lPtr)
    {
        assert(lPtr != nullptr);
        if (IsEqualObject(lPtr))
        {
            return m_Ptr;
        }

        // Smart pointer might not have been initialized by now
        if(m_Ptr)
        {
            m_Ptr->Release();            
        }

        lPtr->AddRef();
        m_Ptr = lPtr;
        return m_Ptr;
    }

    INTERFACE* operator=(IUnknown* pIUnknown)
    {
        assert(pIUnknown != nullptr);
        assert(piid != nullptr);
        pIUnknown->QueryInterface(*piid, reinterpret_cast<void**>(&m_Ptr));
        assert(m_Ptr != nullptr);
        return m_Ptr;
    }

    INTERFACE* operator=(const ComSmartPtr<INTERFACE, piid>& RefComPtr)
    {
        assert(&RefComPtr != nullptr);
        // using static cast here since conversion operator is explicitly defined
        m_Ptr = static_cast<INTERFACE*>(RefComPtr);

        if (m_Ptr)
        {
            m_Ptr->AddRef();
        }
        return m_Ptr;
    }

    void Attach(INTERFACE* lPtr)
    {
        if (lPtr)
        {
            if (m_Ptr)
            {
                m_Ptr->Release();
                m_Ptr = nullptr;
            }
            m_Ptr = lPtr;
        }
    }

    INTERFACE* Detach()
    {
        INTERFACE* lPtr = m_Ptr;
        m_Ptr = nullptr;
        return lPtr;
    }

    void Release()
    {
        if (m_Ptr)
        {
            m_Ptr->Release();
            m_Ptr = nullptr;
        }
    }

    // Partial implementation of CComPtrBase::CoCreateInstance
    // http://msdn.microsoft.com/en-us/library/aa263131(v=vs.60).aspx
    //
    // rclsid is of form CLSID_CLASS e.g. "CLSID_SpStream". Templated INTERFACE type is an ICLASS derivative from CLASS e.g. "ISpStream".
    // dwClsContext defaults to CLSCTX_INPROC_SERVER.
    HRESULT CoCreateInstance(REFCLSID rclsid)
    {
        return ::CoCreateInstance(rclsid, NULL, CLSCTX_INPROC_SERVER, __uuidof(INTERFACE), reinterpret_cast<void**>(&m_Ptr));
    }

    // Implementation of CComPtrBase::CopyTo
    // http://msdn.microsoft.com/ru-ru/library/1wesxec9.aspx
    // http://msdn.microsoft.com/en-us/library/windows/desktop/aa378137(v=vs.85).aspx
    HRESULT CopyTo(INTERFACE** pplPtr)
    {
        assert(pplPtr != nullptr);

        if(pplPtr == nullptr)
        {
            return E_POINTER;
        }

        if (m_Ptr != *pplPtr)
        {
            *pplPtr = m_Ptr;
            m_Ptr->AddRef();
        }

        return S_OK;
    }

    bool IsEqualObject(IUnknown* pOther)
    {
        assert(pOther != nullptr);
        if(m_Ptr)
        {
            IUnknown* pUnknown = nullptr;
            m_Ptr->QueryInterface(IID_IUnknown, reinterpret_cast<void**>(&pUnknown));
            bool result = (pOther == pUnknown) ? true : false;
            pUnknown->Release();
            return result;
        }
        return false;
    }

private:
    INTERFACE* m_Ptr;
};

#endif // _COMSMARTPTR_H_
