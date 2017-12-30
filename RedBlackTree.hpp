#ifndef REDBLACKTREE_HPP
#define REDBLACKTREE_HPP

template<typename Key>
class RB_node
{
public:

	RB_node(const Key&key , RB_node* p = nullptr , RB_node* l = nullptr , RB_node* r = nullptr)
		:k(key) , par(p) , left(l) , right(r)
	{   }

	RB_node()
		:k(Key()) , par(nullptr) , left(nullptr) , right(nullptr) , isred(false)
	{   }

	Key& getkey()
	{
		return k;
	}

	Key k;
	bool isred;
	RB_node* par;
	RB_node* left;
	RB_node* right;
};

template<typename Key,class nodetype>
class RB_Tree
{
public:
	using key_type = Key;
	using nodeptr = nodetype * ;

	RB_Tree()
	{
		nil = &nilnode;
		root = nil;
	}

	nodeptr& getroot()
	{
		return root;
	}

	int back_height()
	{
		nodeptr np = root;
		int back_count = 0;
		for (; np != nil;)
		{
			if (!np->isred)
				++back_count;
			np = np->left();
		}
		return back_count;
	}

	RB_Tree& insert(key_type& key)
	{
		nodeptr np = new nodetype (key);
		insert(np);
		return *this;
	}

	RB_Tree& insert(nodeptr np)
	{
		nodeptr parent = nil;
		for (nodeptr temp = root; temp != nil;)
		{
			parent = temp;
			if (np->getkey() < temp->getkey())
				temp = temp->left;
			else
				temp = temp->right;
		}
		np->par = parent;
		if (parent == nil)
			root = np;
		else if (np->getkey() < parent->getkey())
			parent->left = np;
		else
			parent->right = np;
		np->right = nil;
		np->left = nil;
		np->isred = true;
		insert_fixup(np);

		return *this;
	}

	nodeptr find(const key_type& key) const
	{
		nodeptr now = root;
		for (; now;)
		{
			if (key == now->getkey())
				break;
			else if (key < now->getkey())
				now = now->left;
			else
				now = now->right;
		}
		return now;
	}

	bool deletekey(const key_type& key)
	{
		nodeptr np = find(key);
		if (np)
		{
			erase(np);
			return true;
		}
		else
			return false;
	}

	RB_Tree& erase(nodeptr z)
	{
		nodeptr x;
		nodeptr y = z;
		bool y_original_color = y->isred;
		if (z->left == nil)
		{
			x = z->right;
			transplant(z , z->right);
		}
		else if (z->right == nil)
		{
			x = z->left;
			transplant(z , z->left);
		}
		else
		{
			y = minimum(z->right);
			y_original_color = y->isred;
			x = y->right;
			if (y->par == z)
				x->par = y;
			else
			{
				transplant(y , y->right);
				y->right = z->right;
				y->right->par = y;
			}
			transplant(z , y);
			y->left = z->left;
			y->left->par = y;
			y->isred = z->isred;
		}
		if (y_original_color == false)
			delete_fixup(x);

		delete z;
		return *this;
	}

	void clear()
	{
		cleartree(root);
		root = nil;
	}

private:

	void insert_fixup(nodeptr np)
	{
		for (; np->par->isred;)
		{

			nodeptr parbro;
			if (np->par == np->par->par->left)
			{
				parbro = np->par->par->right;

				if (parbro->isred)
				{
					np->par->isred = false;
					parbro->isred = false;
					np->par->par->isred = true;
					np = np->par->par;
				}
				else
				{
					if (np == np->par->right)
					{
						np = np->par;
						leftrotate(np);
					}
					np->par->isred = false;
					np->par->par->isred = true;
					rightrotate(np->par->par);
				}
			}
			else
			{
				parbro = np->par->par->left;
				if (parbro->isred)
				{
					np->par->isred = false;
					parbro->isred = false;
					np->par->par->isred = true;
					np = np->par->par;
				}
				else
				{
					if (np == np->par->left)
					{
						np = np->par;
						rightrotate(np);
					}
					np->par->isred = false;
					np->par->par->isred = true;
					leftrotate(np->par->par);
				}
			}
		}
		root->isred = false;
	}

	void leftrotate(nodeptr np)
	{
		nodeptr oth = np->right;
		np->right = oth->left;

		if (oth->left != nil)
			oth->left->par = np;

		oth->par = np->par;
		if (np->par == nil)
			root = oth;
		else if (np == np->par->left)
			np->par->left = oth;
		else
			np->par->right = oth;

		oth->left = np;
		np->par = oth;
	}

	void rightrotate(nodeptr np)
	{
		nodeptr oth = np->left;
		np->left = oth->right;

		if (oth->right != nil)
			oth->right->par = np;

		oth->par = np->par;
		if (np->par == nil)
			root = oth;
		else if (np == np->par->left)
			np->par->left = oth;
		else
			np->par->right = oth;

		oth->right = np;
		np->par = oth;
	}

	void transplant(nodeptr x , nodeptr y)
	{
		if (x->par == nil)
			root = y;
		else if (x == x->par->left)
			x->par->left = y;
		else
			x->par->right = y;

		y->par = x->par;
	}

	void delete_fixup(nodeptr x)
	{
		for (; x != root && (x->isred == false);)
		{
			nodeptr bro;
			if (x == x->par->left)
			{
				bro = x->par->right;
				if (bro->isred)
				{
					bro->isred = false;
					x->par->isred = true;
					leftrotate(x->par);
					bro = x->par->right;
				}
				if ((bro->left->isred == false) && (bro->right->isred == false))
				{
					bro->isred = true;
					x = x->par;
				}
				else
				{
					if (bro->right->isred == false)
					{
						bro->left->isred = false;
						bro->isred = true;
						rightrotate(bro);
						bro = x->par->right;
					}
					bro->isred = x->par->isred;
					x->par->isred = false;
					bro->right->isred = false;
					leftrotate(x->par);
					x = root;
				}
			}
			else
			{
				bro = x->par->left;
				if (bro->isred)
				{
					bro->isred = false;
					x->par->isred = true;
					rightrotate(x->par);
					bro = x->par->left;
				}
				if ((bro->right->isred == false) && (bro->left->isred == false))
				{
					bro->isred = true;
					x = x->par;
				}
				else
				{
					if (bro->left->isred == false)
					{
						bro->right->isred = false;
						bro->isred = true;
						leftrotate(bro);
						bro = x->par->left;
					}
					bro->isred = x->par->isred;
					x->par->isred = false;
					bro->left->isred = false;
					rightrotate(x->par);
					x = root;
				}
			}
		}
		x->isred = false;
	}

	nodeptr minimum(nodeptr np)
	{
		for (;;)
		{
			if (np->left != nil)
				np = np->left;
			else
				return np;
		}
	}

	nodeptr maxmum(nodeptr np)
	{
		for (;;)
		{
			if (np->right)
				np = np->right;
			else
				return np;
		}
	}

	void cleartree(nodeptr np)
	{
		if (np == nil)
			return;

		cleartree(np->left);
		cleartree(np->right);
		delete np;
	}

	nodeptr root = nullptr;
	nodetype nilnode;
	nodeptr nil;
};


#endif //REDBLACKTREE_HPP