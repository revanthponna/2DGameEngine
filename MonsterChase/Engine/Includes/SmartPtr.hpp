#pragma once

#include "Matrix.hpp"

struct tCounter {

	UInt32 uNumSmartRef = 0;
	UInt32 uNumWeakRef = 0;
};

template <class Type>
class WeakPtr;

template <class Type>
class SmartPtr {

	public:
						SmartPtr	() noexcept;
						SmartPtr	(Type* pData) noexcept;
						SmartPtr	(const SmartPtr<Type> & pSmartPtr) noexcept; 
						SmartPtr	(SmartPtr<Type>&& pSmartPtr) noexcept;
						SmartPtr	(const WeakPtr<Type>& pWeakPtr) noexcept;

						~SmartPtr   () noexcept;

		Type*			operator->	() noexcept;

		template <class ParamType>
		SmartPtr<Type>& operator=	(const SmartPtr<ParamType>& pSmartPtr) noexcept;

		template <class ParamType>
		SmartPtr<Type>& operator=	(SmartPtr<ParamType>&& pSmartPtr) noexcept;

		bool			operator==	(const SmartPtr<Type>& pParam) const noexcept;

		Type*			GetData		() noexcept;

private:

	Type     *  vData;
	tCounter *  vRefCount;
};

template <class Type>
SmartPtr<Type>::SmartPtr() noexcept {

	vData = nullptr;
	vRefCount = nullptr;
}

template <class Type>
SmartPtr<Type>::SmartPtr(Type* pData) noexcept {

	vData = pData;
	vRefCount = new tCounter();

	vRefCount->uNumSmartRef = 1;
}

template <class Type>
SmartPtr<Type>::SmartPtr(const SmartPtr<Type>& pSmartPtr) noexcept {

	vData = pSmartPtr.vData;
	vRefCount = pSmartPtr.vRefCount;

	++vRefCount->uNumSmartRef;
}

template <class Type>
SmartPtr<Type>::SmartPtr(SmartPtr<Type>&& pSmartPtr) noexcept {

	vData = pSmartPtr.vData;
	vRefCount = pSmartPtr.vRefCount;

	pSmartPtr.vData = nullptr;
	pSmartPtr.vRefCount = nullptr;
}


template <class Type>
SmartPtr<Type>::SmartPtr(const WeakPtr<Type>& pWeakPtr) noexcept {

	if (!pWeakPtr.vRefCount->uNumSmartRef) {

		vData = nullptr;
		vRefCount = nullptr;

		return;
	}

	vData = pWeakPtr.vData;
	vRefCount = pWeakPtr.vRefCount;

	++vRefCount->uNumSmartRef;
}

template<class Type>
SmartPtr<Type>::~SmartPtr() noexcept
{
	if (vRefCount) {

		--vRefCount->uNumSmartRef;

		if (!vRefCount->uNumSmartRef) {

			delete vData;

			if (!vRefCount->uNumWeakRef)
				delete vRefCount;
		}
	}
}

template<class Type>
Type*
SmartPtr<Type>::operator->() noexcept
{
	return vData;
}

template <class Type>
template <class ParamType>
SmartPtr<Type>&
SmartPtr<Type>::operator=(const SmartPtr<ParamType>& pSmartPtr) noexcept
{
	if (vRefCount) {

		--vRefCount->uNumSmartRef;

		if (!vRefCount->uNumSmartRef) {

			delete vData;

			if (!vRefCount->uNumWeakRef)
				delete vRefCount;
		}
	}

	vData = pSmartPtr.vData;
	vRefCount = pSmartPtr.vRefCount;

	if (pSmartPtr.vRefCount)
		++vRefCount->uNumSmartRef;

	return *this;
}

template <class Type>
template <class ParamType>
SmartPtr<Type>&
SmartPtr<Type>::operator= (SmartPtr<ParamType>&& pSmartPtr) noexcept
{
	if (vRefCount) {

		--vRefCount->uNumSmartRef;

		if (!vRefCount->uNumSmartRef) {

			delete vData;

			if (!vRefCount->uNumWeakRef)
				delete vRefCount;
		}
	}

	vData = pSmartPtr.vData;
	vRefCount = pSmartPtr.vRefCount;

	pSmartPtr.vData = nullptr;
	pSmartPtr.vRefCount = nullptr;

	return *this;
}

template <class Type>
bool
SmartPtr<Type>::operator==(const SmartPtr<Type>& pParam) const noexcept
{
	if (vData == pParam.vData)
		return true;
	else
		return false;
}

template <class Type>
Type*
SmartPtr<Type>::GetData() noexcept
{
	return vData;
}
