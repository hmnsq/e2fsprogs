/*
  Red Black Trees
  (C) 1999  Andrea Arcangeli <andrea@suse.de>
  (C) 2002  David Woodhouse <dwmw2@infradead.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

  linux/lib/rbtree.c
*/

#ضمن "rbtree.h"

ساكن فراغ __rb_rotate_left(بنية rb_غوټه *غوټه, بنية rb_ریښه *ریښه)
{
	بنية rb_غوټه *ښی = غوټه->rb_ښی;
	بنية rb_غوټه *پلار = ext2fs_rb_پلار(غوټه);

	إذا ((غوټه->rb_ښی = ښی->rb_کیڼ))
		ext2fs_rb_سېټ_پلار(ښی->rb_کیڼ, غوټه);
	ښی->rb_کیڼ = غوټه;

	ext2fs_rb_سېټ_پلار(ښی, پلار);

	إذا (پلار)
	{
		إذا (غوټه == پلار->rb_کیڼ)
			پلار->rb_کیڼ = ښی;
		وإلا
			پلار->rb_ښی = ښی;
	}
	وإلا
		ریښه->rb_غوټه = ښی;
	ext2fs_rb_سېټ_پلار(غوټه, ښی);
}

ساكن فراغ __rb_rotate_right(بنية rb_غوټه *غوټه, بنية rb_ریښه *ریښه)
{
	بنية rb_غوټه *کیڼ = غوټه->rb_کیڼ;
	بنية rb_غوټه *پلار = ext2fs_rb_پلار(غوټه);

	إذا ((غوټه->rb_کیڼ = کیڼ->rb_ښی))
		ext2fs_rb_سېټ_پلار(کیڼ->rb_ښی, غوټه);
	کیڼ->rb_ښی = غوټه;

	ext2fs_rb_سېټ_پلار(کیڼ, پلار);

	إذا (پلار)
	{
		إذا (غوټه == پلار->rb_ښی)
			پلار->rb_ښی = کیڼ;
		وإلا
			پلار->rb_کیڼ = کیڼ;
	}
	وإلا
		ریښه->rb_غوټه = کیڼ;
	ext2fs_rb_سېټ_پلار(غوټه, کیڼ);
}

فراغ ext2fs_rb_ورننباسه_color(بنية rb_غوټه *غوټه, بنية rb_ریښه *ریښه)
{
	بنية rb_غوټه *پلار, *gparent;

	طالما ((پلار = ext2fs_rb_پلار(غوټه)) && ext2fs_rb_is_red(پلار))
	{
		gparent = ext2fs_rb_پلار(پلار);

		إذا (پلار == gparent->rb_کیڼ)
		{
			{
				سجل بنية rb_غوټه *uncle = gparent->rb_ښی;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_سېټ_black(uncle);
					ext2fs_rb_سېټ_black(پلار);
					ext2fs_rb_سېټ_red(gparent);
					غوټه = gparent;
					استمر;
				}
			}

			إذا (پلار->rb_ښی == غوټه)
			{
				سجل بنية rb_غوټه *tmp;
				__rb_rotate_left(پلار, ریښه);
				tmp = پلار;
				پلار = غوټه;
				غوټه = tmp;
			}

			ext2fs_rb_سېټ_black(پلار);
			ext2fs_rb_سېټ_red(gparent);
			__rb_rotate_right(gparent, ریښه);
		} وإلا {
			{
				سجل بنية rb_غوټه *uncle = gparent->rb_کیڼ;
				إذا (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_سېټ_black(uncle);
					ext2fs_rb_سېټ_black(پلار);
					ext2fs_rb_سېټ_red(gparent);
					غوټه = gparent;
					استمر;
				}
			}

			إذا (پلار->rb_کیڼ == غوټه)
			{
				سجل بنية rb_غوټه *tmp;
				__rb_rotate_right(پلار, ریښه);
				tmp = پلار;
				پلار = غوټه;
				غوټه = tmp;
			}

			ext2fs_rb_سېټ_black(پلار);
			ext2fs_rb_سېټ_red(gparent);
			__rb_rotate_left(gparent, ریښه);
		}
	}

	ext2fs_rb_سېټ_black(ریښه->rb_غوټه);
}

ساكن فراغ __rb_erase_color(بنية rb_غوټه *غوټه, بنية rb_غوټه *پلار,
			     بنية rb_ریښه *ریښه)
{
	بنية rb_غوټه *other;

	طالما ((!غوټه || ext2fs_rb_is_black(غوټه)) && غوټه != ریښه->rb_غوټه)
	{
		إذا (پلار->rb_کیڼ == غوټه)
		{
			other = پلار->rb_ښی;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_سېټ_black(other);
				ext2fs_rb_سېټ_red(پلار);
				__rb_rotate_left(پلار, ریښه);
				other = پلار->rb_ښی;
			}
			إذا ((!other->rb_کیڼ || ext2fs_rb_is_black(other->rb_کیڼ)) &&
			    (!other->rb_ښی || ext2fs_rb_is_black(other->rb_ښی)))
			{
				ext2fs_rb_سېټ_red(other);
				غوټه = پلار;
				پلار = ext2fs_rb_پلار(غوټه);
			}
			وإلا
			{
				إذا (!other->rb_ښی || ext2fs_rb_is_black(other->rb_ښی))
				{
					ext2fs_rb_سېټ_black(other->rb_کیڼ);
					ext2fs_rb_سېټ_red(other);
					__rb_rotate_right(other, ریښه);
					other = پلار->rb_ښی;
				}
				ext2fs_rb_سېټ_color(other, ext2fs_rb_color(پلار));
				ext2fs_rb_سېټ_black(پلار);
				ext2fs_rb_سېټ_black(other->rb_ښی);
				__rb_rotate_left(پلار, ریښه);
				غوټه = ریښه->rb_غوټه;
				اكسر;
			}
		}
		وإلا
		{
			other = پلار->rb_کیڼ;
			إذا (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_سېټ_black(other);
				ext2fs_rb_سېټ_red(پلار);
				__rb_rotate_right(پلار, ریښه);
				other = پلار->rb_کیڼ;
			}
			إذا ((!other->rb_کیڼ || ext2fs_rb_is_black(other->rb_کیڼ)) &&
			    (!other->rb_ښی || ext2fs_rb_is_black(other->rb_ښی)))
			{
				ext2fs_rb_سېټ_red(other);
				غوټه = پلار;
				پلار = ext2fs_rb_پلار(غوټه);
			}
			وإلا
			{
				إذا (!other->rb_کیڼ || ext2fs_rb_is_black(other->rb_کیڼ))
				{
					ext2fs_rb_سېټ_black(other->rb_ښی);
					ext2fs_rb_سېټ_red(other);
					__rb_rotate_left(other, ریښه);
					other = پلار->rb_کیڼ;
				}
				ext2fs_rb_سېټ_color(other, ext2fs_rb_color(پلار));
				ext2fs_rb_سېټ_black(پلار);
				ext2fs_rb_سېټ_black(other->rb_کیڼ);
				__rb_rotate_right(پلار, ریښه);
				غوټه = ریښه->rb_غوټه;
				اكسر;
			}
		}
	}
	إذا (غوټه)
		ext2fs_rb_سېټ_black(غوټه);
}

فراغ ext2fs_rb_erase(بنية rb_غوټه *غوټه, بنية rb_ریښه *ریښه)
{
	بنية rb_غوټه *ماشوم, *پلار;
	صحيح color;

	إذا (!غوټه->rb_کیڼ)
		ماشوم = غوټه->rb_ښی;
	وإلا إذا (!غوټه->rb_ښی)
		ماشوم = غوټه->rb_کیڼ;
	وإلا
	{
		بنية rb_غوټه *old = غوټه, *کیڼ;

		غوټه = غوټه->rb_ښی;
		طالما ((کیڼ = غوټه->rb_کیڼ) != عدم)
			غوټه = کیڼ;

		إذا (ext2fs_rb_پلار(old)) {
			إذا (ext2fs_rb_پلار(old)->rb_کیڼ == old)
				ext2fs_rb_پلار(old)->rb_کیڼ = غوټه;
			وإلا
				ext2fs_rb_پلار(old)->rb_ښی = غوټه;
		} وإلا
			ریښه->rb_غوټه = غوټه;

		ماشوم = غوټه->rb_ښی;
		پلار = ext2fs_rb_پلار(غوټه);
		color = ext2fs_rb_color(غوټه);

		إذا (پلار == old) {
			پلار = غوټه;
		} وإلا {
			إذا (ماشوم)
				ext2fs_rb_سېټ_پلار(ماشوم, پلار);
			پلار->rb_کیڼ = ماشوم;

			غوټه->rb_ښی = old->rb_ښی;
			ext2fs_rb_سېټ_پلار(old->rb_ښی, غوټه);
		}

		غوټه->rb_پلار_color = old->rb_پلار_color;
		غوټه->rb_کیڼ = old->rb_کیڼ;
		ext2fs_rb_سېټ_پلار(old->rb_کیڼ, غوټه);

		اذهب color;
	}

	پلار = ext2fs_rb_پلار(غوټه);
	color = ext2fs_rb_color(غوټه);

	إذا (ماشوم)
		ext2fs_rb_سېټ_پلار(ماشوم, پلار);
	إذا (پلار)
	{
		إذا (پلار->rb_کیڼ == غوټه)
			پلار->rb_کیڼ = ماشوم;
		وإلا
			پلار->rb_ښی = ماشوم;
	}
	وإلا
		ریښه->rb_غوټه = ماشوم;

 color:
	إذا (color == RB_BLACK)
		__rb_erase_color(ماشوم, پلار, ریښه);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
بنية rb_غوټه *ext2fs_rb_لومړی(ثابت بنية rb_ریښه *ریښه)
{
	بنية rb_غوټه	*n;

	n = ریښه->rb_غوټه;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_کیڼ)
		n = n->rb_کیڼ;
	أرجع n;
}

بنية rb_غوټه *ext2fs_rb_وروستی(ثابت بنية rb_ریښه *ریښه)
{
	بنية rb_غوټه	*n;

	n = ریښه->rb_غوټه;
	إذا (!n)
		أرجع عدم;
	طالما (n->rb_ښی)
		n = n->rb_ښی;
	أرجع n;
}

بنية rb_غوټه *ext2fs_rb_بل(بنية rb_غوټه *غوټه)
{
	بنية rb_غوټه *پلار;

	إذا (ext2fs_rb_پلار(غوټه) == غوټه)
		أرجع عدم;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	إذا (غوټه->rb_ښی) {
		غوټه = غوټه->rb_ښی;
		طالما (غوټه->rb_کیڼ)
			غوټه=غوټه->rb_کیڼ;
		أرجع (بنية rb_غوټه *)غوټه;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	طالما ((پلار = ext2fs_rb_پلار(غوټه)) && غوټه == پلار->rb_ښی)
		غوټه = پلار;

	أرجع پلار;
}

بنية rb_غوټه *ext2fs_rb_prev(بنية rb_غوټه *غوټه)
{
	بنية rb_غوټه *پلار;

	إذا (ext2fs_rb_پلار(غوټه) == غوټه)
		أرجع عدم;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	إذا (غوټه->rb_کیڼ) {
		غوټه = غوټه->rb_کیڼ;
		طالما (غوټه->rb_ښی)
			غوټه=غوټه->rb_ښی;
		أرجع (بنية rb_غوټه *)غوټه;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	طالما ((پلار = ext2fs_rb_پلار(غوټه)) && غوټه == پلار->rb_کیڼ)
		غوټه = پلار;

	أرجع پلار;
}

فراغ ext2fs_rb_replace_غوټه(بنية rb_غوټه *victim, بنية rb_غوټه *new,
			  بنية rb_ریښه *ریښه)
{
	بنية rb_غوټه *پلار = ext2fs_rb_پلار(victim);

	/* Set the surrounding nodes to point to the replacement */
	إذا (پلار) {
		إذا (victim == پلار->rb_کیڼ)
			پلار->rb_کیڼ = new;
		وإلا
			پلار->rb_ښی = new;
	} وإلا {
		ریښه->rb_غوټه = new;
	}
	إذا (victim->rb_کیڼ)
		ext2fs_rb_سېټ_پلار(victim->rb_کیڼ, new);
	إذا (victim->rb_ښی)
		ext2fs_rb_سېټ_پلار(victim->rb_ښی, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}
