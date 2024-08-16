#pragma once

#include "SmartPtr.hpp"

template <class Type>
class WeakPtr {

	public:

						WeakPtr		() noexcept;
						WeakPtr		(WeakPtr<Type>& pSmartPtr) noexcept;
						WeakPtr		(WeakPtr<Type>&& pSmartPtr) noexcept;
						WeakPtr		(SmartPtr<Type>& pWeakPtr) noexcept;

						~WeakPtr	() noexcept;

	template <class ParamType>
	WeakPtr<Type>&		operator=	(const WeakPtr<ParamType>& pSmartPtr) noexcept;

	template <class ParamType>
	WeakPtr<Type>&		operator=	(WeakPtr<ParamType>&& pSmartPtr) noexcept;

	bool				operator==	(const WeakPtr<Type>& pParam) const noexcept;

private:

	Type     * vData;
	tCounter * vRefCount;
};

template <class Type>
WeakPtr<Type>::WeakPtr() noexcept {

	vData = nullptr;
	vRefCount = nullptr;
}

template <class Type>
WeakPtr<Type>::WeakPtr(WeakPtr<Type>& pSmartPtr) noexcept {

	if (!pSmartPtr.vData) {

		vData = nullptr;
		vRefCount = nullptr;

		return;
	}

	vData = pSmartPtr.vData;
	vRefCount = pSmartPtr.vRefCount;

	++vRefCount->uNumWeakRef;
}

template <class Type>
WeakPtr<Type>::WeakPtr(WeakPtr<Type>&& pSmartPtr) noexcept {

	vData = pSmartPtr.vData;
	vRefCount = pSmartPtr.vRefCount;

	pSmartPtr.vData = nullptr;
	pSmartPtr.vRefCount = nullptr;
}

template <class Type>
WeakPtr<Type>::WeakPtr(SmartPtr<Type>& pWeakPtr) noexcept {

	if (!pWeakPtr.vData) {

		vData = nullptr;
		vRefCount = nullptr;

		return;
	}

	vData = pWeakPtr.vData;
	vRefCount = pWeakPtr.vRefCount;

	++vRefCount->uNumWeakRef;
}

template<class Type>
WeakPtr<Type>::~WeakPtr() noexcept
{
	if (vRefCount) {

		--vRefCount->uNumWeakRef;

		if (!vRefCount->uNumWeakRef && !!vRefCount->uNumSmartRef)
			delete vRefCount;
	}
}

template <class Type>
template <class ParamType>
WeakPtr<Type>&
WeakPtr<Type>::operator=(const WeakPtr<ParamType>& pWeakPtr) noexcept
{
	if (vRefCount) {

		--vRefCount->uNumWeakRef;

		if (!vRefCount->uNumWeakRef && !vRefCount->uNumSmartRef)
			delete vRefCount;
	}

	vData = pWeakPtr.vData;
	vRefCount = pWeakPtr.vRefCount;

	if (vRefCount)
		++vRefCount->uNumWeakRef;

	return *this;
}

template <class Type>
template <class ParamType>
WeakPtr<Type>&
WeakPtr<Type>::operator=(WeakPtr<ParamType>&& pWeakPtr) noexcept
{
	if (vRefCount) {

		--vRefCount->uNumWeakRef;

		if (!vRefCount->uNumWeakRef && !vRefCount->uNumSmartRef)
			delete vRefCount;
	}

	vData = pWeakPtr.vData;
	vRefCount = pWeakPtr.vRefCount;

	pWeakPtr.vData = nullptr;
	pWeakPtr.vRefCount = nullptr;

	return *this;
}

template <class Type>
bool
WeakPtr<Type>::operator==(const WeakPtr<Type>& pParam) const noexcept
{
	if (vData == pParam.vData)
		return true;
	else
		return false;
}
