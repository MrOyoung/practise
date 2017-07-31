#include "../include/slist.h"


struct slist *create_slist()
{
	struct slist *head;

	head = (struct slist)calloc(1, sizeof(struct slist));
	if (NULL == head)
	{
		perror("calloc error");
		exit(-1);
	}

	head->next = NULL;

	return head;
}

int append_slist_node(struct slist *head,struct MODULE_LOG_INFO node)
{
	if (NULL == head)
	{
		printf("head is empty\n");
		return -1;
	}

	struct slist *plist = head;
	struct slist *new_node;

	new_node = (struct slist *)calloc(1, sizeof(struct slist));
	if (NULL == new_node)
	{
		perror("calloc error");
		exit(-1);
	}

	memcpy(&new_node->info, &node, sizeof(struct MODULE_LOG_INFO));
	new_node->next = NULL;

	while (plist->next)
	{
		plist = plist->next;
	}
	
	plist->next = new_node;

	return 0;
}

static struct slist *serach_slist_node(static slist *head, int name)
{
	if (NULL == head)		
	{
		printf("head is empty\n");
		return -1;
	}
	
	struct slist *plist = head;

	while (plist->next)
	{
		plist = plist->next;	
		
		if (plist->module_name == name)
		{
			return plist;
		}

	}
	
	return NULL;
}


int destory_slist(struct slist *head)
{
	if (NULL == head)		
	{
		printf("head is empty\n");
		return -1;
	}
	
	struct slist *pnode = head;
	struct slist *pnext = head;
	
	while(pnode)
	{
			
	}

}
