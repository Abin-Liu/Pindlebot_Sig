#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <windows.h>

class LinkedItem
{
	friend class LinkedList;
public:
	LinkedItem();
	~LinkedItem();

	LPVOID GetData() const;
	void SetData(LPVOID pData);

protected:
	LinkedItem*		pPrev;
	LinkedItem*		pNext;
	LPVOID			lpData;
};

BOOL __stdcall IsValidListItem(const LinkedItem* li);

class LinkedList
{
public:
	BOOL IsEmpty() const;
	LinkedList();
	virtual ~LinkedList();
	virtual LinkedItem* AddItem(LPVOID lpData);
	virtual LinkedItem* InsertItem(LinkedItem* pItem, LPVOID lpData);
	virtual LinkedItem* RemoveItem(LinkedItem* pItem);
	LinkedItem* GetFirstItem();
	LinkedItem* GetLastItem();
	LinkedItem* GetPrevItem(const LinkedItem* pItem);
	LinkedItem* GetNextItem(const LinkedItem* pItem);
	UINT GetItemCount() const;
	virtual void RemoveAll();

protected:
	LinkedItem*		pFirst;
	LinkedItem*		pLast;
	UINT			cItemCount;
};

#endif

