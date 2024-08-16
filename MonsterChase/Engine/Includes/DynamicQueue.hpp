#pragma once

#include "Moveable.hpp"

// node structure that is used in building Dynamic Queue.

template <typename NodeType>
struct tNode {

	NodeType *        uData     = nullptr;
    tNode<NodeType> * uNxtElem  = nullptr;
};

// Doubly-ended Dynamic Queue

template <typename NodeType>
class DynamicQueue {

	public:

		template <typename ParamType = void>
		using WalkFunc = bool (*) (tNode<NodeType> * pPrevNode, tNode<NodeType> * pCurNode, ParamType * pContext);

						DynamicQueue		();
						~DynamicQueue		() noexcept;


		void			Post				(NodeType * pElem);
		NodeType * 		Pick				() noexcept;

		void			CleanUp				(UInt64 pMaxNodesAllowed) noexcept;				

		template <typename ParamType, typename CtxType = void>
		void			WalkQueue			(WalkFunc<ParamType> pFunc, CtxType * pContext) noexcept;

		UInt64			GetNumElems			() const noexcept;

	private:

		tNode<NodeType> * vHead;
		tNode<NodeType> * vTail;
		UInt64			  vNumNodes;

};

template <typename NodeType>
DynamicQueue<NodeType>::DynamicQueue () 
{
    vHead       = new tNode<NodeType> ();
    vTail       = vHead;
    vNumNodes   = 0;
}

template <typename NodeType>
DynamicQueue<NodeType>::~DynamicQueue () noexcept
{
    tNode<NodeType> * currnode;
    tNode<NodeType> * tempnode;

    if (!vHead)
        return;

    currnode = vHead->uNxtElem;

    // walk and delete all nodes.
    while (currnode != nullptr) {

        tempnode = currnode;
        currnode = currnode->uNxtElem;

        delete tempnode;
    }

    delete vHead;     // delete dummy node

    vHead = nullptr;
    vTail = nullptr;
}

template <typename NodeType>
void
DynamicQueue<NodeType>::Post (NodeType * pElem) 
{
    tNode<NodeType> * tempnode;

    // don't insert null into the queue
    if (pElem == nullptr)
        return;

    //create a new node to add to the queue
    tempnode = new tNode<NodeType> ();

    tempnode->uData = pElem;    // fill the data

    // Insert at the tail.
    vTail->uNxtElem = tempnode;
    vTail           = tempnode;

    ++vNumNodes;
}

template <typename NodeType>
NodeType *
DynamicQueue<NodeType>::Pick () noexcept
{
    tNode<NodeType> * node;
    tNode<NodeType> * newhead;
    NodeType *        data;

    node    = vHead;
    newhead = node->uNxtElem;

    if (!newhead)
        return nullptr;     // queue is empty

    data  = newhead->uData;
    vHead = newhead;        // shift the head forward

    delete node;

    --vNumNodes;

    return data;
}

template <typename NodeType>
template <typename ParamType, typename CtxType>
void			
DynamicQueue<NodeType>::WalkQueue (WalkFunc<ParamType> pFunc, CtxType * pContext) noexcept
{
    tNode<NodeType> *  prev_node;
    tNode<NodeType> *  curr_node;

    if (vHead == vTail)     // empty queue
        return;

    prev_node = vHead;
    curr_node = vHead->uNxtElem;

    // Walk through the queue till last node
    while (curr_node != nullptr) {

        // Break if false is returned from the callback function
        // (in-case user want to stop the walk for some condition, before reaching the end.)

        if (!pFunc (prev_node, curr_node, pContext))
            break;

        prev_node = curr_node;
        curr_node = curr_node->uNxtElem;
    }    
}

template <typename NodeType>
UInt64			
DynamicQueue<NodeType>::GetNumElems () const noexcept
{
    return vNumNodes;
}

template <typename NodeType>
void			
DynamicQueue<NodeType>::CleanUp (UInt64 pMaxNodesAllowed) noexcept
{
    NodeType * elem;

    // deletes the elements in the start till the no. of nodes reaches pMaxNodesAllowed.

    while (vNumNodes > pMaxNodesAllowed) {

        elem = Pick ();

        delete elem;
    }
}
