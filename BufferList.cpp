#include "BufferList.H"
// _group_by_values WILL BE A NUL DELIMITED STRING representing the values in an internal format
BufferList::BufferList(char *group_by, int group_by_size)
{
//  _max_size = list_size;
  _buffer = list<node>();
  // BAAAAAAAAAAAAAAAAAAD!!!!!!!! YOU DIDNT ALLOCATE MEMORY FOR _group_by_values !!!
  // FIXED but be careful!!! - eddie
  // _group_by_values = (char*) malloc(group_by_size); // could use a memory check
  _group_by_values = new char[group_by_size]; // cuz c++ likes this instead
  memcpy(_group_by_values, group_by, group_by_size);
  _group_by_size = group_by_size;
  gettimeofday(&_last_time_emitted, NULL);
  
}

BufferList::BufferList(char order_att_type)
{
  _buffer = list<node>();

  _order_att_type = order_att_type;
  
  _values_gt_threshold = vector<char*>();
  _curr_slack = 0;
  if (order_att_type == 'i')
    {
      _curr_threshold = new char[sizeof(int)];
      *(int*) _curr_threshold = -999;
    }
  else
    {
      _curr_threshold = new char[sizeof(timeval)];
      
    }
}




BufferList::BufferList(char *group_by_values, int group_by_size, char *group_by_values_str, int group_by_size_str, char order_att_type)
{
  _buffer = list<node>();

  _group_by_values = new char[group_by_size]; // cuz c++ likes this instead
  memcpy(_group_by_values, group_by_values, group_by_size);
  _group_by_size = group_by_size;
  gettimeofday(&_last_time_emitted, NULL);
  
  _group_by_size_str = group_by_size_str;
  _group_by_values_str = new char[group_by_size_str];
  memcpy(_group_by_values_str, group_by_values_str, group_by_size_str);
  _group_by_size = group_by_size;
  _group_by_values = new char[group_by_size];
  memcpy(_group_by_values, group_by_values, group_by_size);
  _order_att_type = order_att_type;
  
  _values_gt_threshold = vector<char*>();
  _curr_slack = 0;
  if (order_att_type == 'i')
    {
      _curr_threshold = new char[sizeof(int)];
      *(int*) _curr_threshold = -999;
    }
  else
    {
      _curr_threshold = new char[sizeof(timeval)];
      
    }
}


int BufferList::size()
{
  return _buffer.size();
}

node BufferList::front()
{
  return _buffer.front();
}

node BufferList::back()
{
  return _buffer.back();
}

list<node>::iterator BufferList::begin()
{
  return _buffer.begin();
}

list<node>::iterator BufferList::end()
{
  return _buffer.end();
}

bool BufferList::isEmpty()
{
  _buffer.empty();
}

void BufferList::push_back(node new_node)
{
  _buffer.push_back(new_node);
}

void BufferList::push_front(node new_node)
{
  _buffer.push_front(new_node);
}

void BufferList::insert(list<node>::iterator iter, node new_node)
{
  _buffer.insert(iter, new_node);
}

node BufferList::pop_front()
{
  node temp = _buffer.front();
  _buffer.pop_front();
  return temp;
}

char* BufferList::getGroupByValues()
{
  return _group_by_values;
}

int BufferList::getGroupBySize()
{
  return _group_by_size;
}

Timestamp BufferList::getLastEmittedTime()
{
  return _last_time_emitted;
}

void BufferList::setLastEmittedTime(Timestamp ts)
{
  _last_time_emitted = ts;
}



// added afterwards 

int BufferList::getGroupByStrSize()
{
  return _group_by_size_str;
}

char* BufferList::getGroupByStr()
{
  return _group_by_values_str;
}

int BufferList::getSlack()
{
  return _curr_slack;
}

void BufferList::increaseSlack()
{
  _curr_slack++;
}

void BufferList::setSlack(int new_slack)
{
  _curr_slack = new_slack;
}

char* BufferList::getThreshold()
{
  return _curr_threshold;
}

void BufferList::setThreshold(char *new_threshold)
{
  _curr_threshold = new_threshold;
}

void BufferList::increaseThreshold()  
{
  if (_order_att_type == 'i')
    {
      int value = *(int*) _curr_threshold;
      value++;
      *(int*) _curr_threshold = value;
    }
  else   
    {
      timeval threshold_struct = *(timeval*) _curr_threshold;
      long value = threshold_struct.tv_sec;
      value += _time_increment;
      timeval new_struct = {value, threshold_struct.tv_usec};
      *(timeval*) _curr_threshold = new_struct;
      // don't know what to do here yet .....
    }
}

int BufferList::countAndClearVector()
{
  _vector_iter = _values_gt_threshold.begin();
  int count = 0;

  while (_vector_iter != _values_gt_threshold.end())
    {
      if (_order_att_type == 'i')
	{
	  int threshold = *(int*) _curr_threshold;
	  int vector_value = *(int*) (*_vector_iter) ;
	  
	  if (vector_value <= threshold)
	    {
	      //cout << "[BufferList] removing from vector ........." << endl;
	      _vector_iter = _values_gt_threshold.erase(_vector_iter);
	    }
	  else
	    {
	      count++;
	      _vector_iter++;
	    }
	}
      else   
	{	  
	  timeval threshold_struct = *(timeval*) _curr_threshold;
	  long threshold = threshold_struct.tv_sec;
	  timeval time_struct = *(timeval*) (*_vector_iter);
	  long vector_seconds = time_struct.tv_sec;
	  
	  if (vector_seconds <= threshold)
	    {
	      //	      cout << "[HashForNewState] removing from vector TIME ........." << endl;
	      _vector_iter = _values_gt_threshold.erase(_vector_iter);
	    }
	  else
	    {
	      count++;
	      _vector_iter++;
	    }
	  // don't know what to do here yet .....
	}        
    }
  //  cout << "count = " << count << "vector.size() = " << _values_gt_threshold.size() << endl;
  return count;
  //  return _values_gt_threshold.size();
  
}

void BufferList::addToVector(char *value)
{
  //  cout << "[BufferList] adding to vector ........." << endl;
  _values_gt_threshold.push_back(value);
}


void BufferList::erase(char *threshold, int size)
{
  if (_order_att_type == 'i')
    {

      int int_threshold = *(int*) threshold;
      node temp_node;
      int int_value = 0;
      
      list<node>::iterator iter = _buffer.begin();
      while (iter != _buffer.end())
	{
	  temp_node = *iter;
	  int_value = *(int*) temp_node.att;
	  
	  if ((int_threshold - int_value) > size)
	    {
	      iter = _buffer.erase(iter);
	    }
	  else
	    {
	      iter++;
	    }
	}
    }
  else
    {
      int int_threshold = *(int*) threshold;
      node temp_node;
      int int_value = 0;
      
      list<node>::iterator iter = _buffer.begin();
      while (iter != _buffer.end())
	{
	  temp_node = *iter;
	  int_value = *(int*) temp_node.att;
	  
	  if ((int_threshold - int_value) > size)
	    {
	      iter = _buffer.erase(iter);
	    }
	  else
	    {
	      iter++;
	    }
	}
      // what goes here?????????
    }
}

//void BufferList::setTimeIncrement(long num)
//{
//  _time_increment = num;
// } 
