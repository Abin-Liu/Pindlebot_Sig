#include "LinkedList.h"

BOOL __stdcall IsValidListItem(const LinkedItem* li)
{
	return (li != NULL &&
		!IsBadReadPtr(li, sizeof(LinkedItem)) &&
		!IsBadWritePtr((LPVOID)li, sizeof(LinkedItem)) );
}

//============================================================================
//	LinkedItem Constructor/Destructor
//============================================================================
LinkedItem::LinkedItem()
{
	pPrev = NULL;
	pNext = NULL;
	lpData = NULL;
}

LinkedItem::~LinkedItem()
{
	if (lpData != NULL)
	{
		delete lpData;
		lpData = NULL;
	}
}

//============================================================================
//	LinkedItem Operations
//============================================================================
LPVOID LinkedItem::GetData() const
{
	return lpData;
}

void LinkedItem::SetData(LPVOID pData)
{
	if(lpData != NULL)
		delete lpData;

	lpData = pData;
}

//============================================================================
//	LinkedList Constructor/Destructor
//============================================================================
LinkedList::LinkedList()
{
	pFirst = NULL;
	pLast = NULL;
	cItemCount = 0;
}

LinkedList::~LinkedList()
{
	RemoveAll();
}

//============================================================================
//	RemoveAll
//----------------------------------------------------------------------------
//============================================================================
void LinkedList::RemoveAll()
{
	LinkedItem* pItem = pFirst;
	while(pItem && IsValidListItem(pItem))
	{
		pItem = RemoveItem(pItem);
	}
}
//============================================================================
//	AddItem
//----------------------------------------------------------------------------
//============================================================================
LinkedItem* LinkedList::AddItem(LPVOID lpData)
{
	return InsertItem(NULL, lpData);
}

//============================================================================
//	InsertItem
//----------------------------------------------------------------------------
// if pItem is NULL, adds to end of list, otherwise adds before pItem
//============================================================================
LinkedItem* LinkedList::InsertItem(LinkedItem* pItem, LPVOID lpData)
{
	if(pItem && !IsValidListItem(pItem))
		return NULL;

	LinkedItem* pnItem = new LinkedItem;
	
	if(!pnItem)
		return NULL;

	pnItem->SetData(lpData);
	pnItem->pNext = pItem;
	if(pItem)
	{
		pnItem->pPrev = pItem->pPrev;

		if(pnItem->pPrev)
			pnItem->pPrev->pNext = pnItem;

		pItem->pPrev = pnItem;
	}
	else
	{
		pnItem->pPrev = pLast;

		if(pLast)
			pLast->pNext = pnItem;

		pLast = pnItem;
	}

	if(pItem == pFirst)
		pFirst = pnItem;

	++cItemCount;

	return pnItem;
}

//============================================================================
//	RemoveItem
//----------------------------------------------------------------------------
//============================================================================
LinkedItem* LinkedList::RemoveItem(LinkedItem* pItem)
{
	if(!pFirst || !pLast || !IsValidListItem(pItem))
		return NULL;

	LinkedItem* pNext = pItem->pNext;

	if(pItem->pPrev)
		pItem->pPrev->pNext = pItem->pNext;

	if(pItem->pNext)
		pItem->pNext->pPrev = pItem->pPrev;

	if(pItem == pFirst)
		pFirst = pItem->pNext;

	if(pItem == pLast)
		pLast = pItem->pPrev;

	delete pItem;
	pItem = NULL;

	--cItemCount;

	return pNext;
}

//============================================================================
//	GetFirstItem
//----------------------------------------------------------------------------
//============================================================================
LinkedItem* LinkedList::GetFirstItem()
{
	return pFirst;
}

//============================================================================
//	GetLastItem
//----------------------------------------------------------------------------
//============================================================================
LinkedItem* LinkedList::GetLastItem()
{
	return pLast;
}

//============================================================================
//	GetPrevItem
//----------------------------------------------------------------------------
//============================================================================
LinkedItem* LinkedList::GetPrevItem(const LinkedItem* pItem)
{
	return (IsValidListItem(pItem)) ? pItem->pPrev : NULL;
}

//============================================================================
//	GetNextItem
//----------------------------------------------------------------------------
//============================================================================
LinkedItem* LinkedList::GetNextItem(const LinkedItem* pItem)
{
	return (IsValidListItem(pItem)) ? pItem->pNext : NULL;
}

//============================================================================
//	GetItemCount
//----------------------------------------------------------------------------
//============================================================================
UINT LinkedList::GetItemCount() const
{
	return cItemCount;
}

BOOL LinkedList::IsEmpty() const
{
	return cItemCount == 0;
}
