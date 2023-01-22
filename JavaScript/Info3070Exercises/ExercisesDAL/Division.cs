using System;
using System.Collections.Generic;

#nullable disable

namespace ExercisesDAL
{
    public partial class Division : SchoolEntity
    {
        public Division()
        {
            Students = new HashSet<Student>();
        }

        //public int Id { get; set; }
        public string Name { get; set; }
        //public byte[] Timer { get; set; }

        public virtual ICollection<Student> Students { get; set; }
    }
}
