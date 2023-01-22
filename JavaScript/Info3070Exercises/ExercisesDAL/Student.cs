using System;
using System.Collections.Generic;

#nullable disable

namespace ExercisesDAL
{
    public partial class Student : SchoolEntity
    {
        //public int Id { get; set; }
        public string Title { get; set; }
        public string FirstName { get; set; }
        public string LastName { get; set; }
        public string PhoneNo { get; set; }
        public string Email { get; set; }
        public int DivisionId { get; set; }
        public byte[] Picture { get; set; }
        // public byte[] Timer { get; set; }

        public virtual Division Division { get; set; }
    }
}
