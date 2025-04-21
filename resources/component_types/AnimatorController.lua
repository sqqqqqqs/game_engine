AnimatorController = {
	hasAnimator = false,

	OnCollisionEnter = function(self, collision)
		if collision.other:GetName() == "player" then
			if self.hasAnimator == false then
				self.hasAnimator = true
				local actor = self.actor
				local anim = actor:AddComponent("Animator")
				anim.sprite_prefix = "box"
				anim.frame_count = 3
				anim.destroy_on_finish = true
				anim.frame_duration = 8
				anim.loops_before_destroy = 3
				
			end

		end
	end,

	OnUpdate = function(self)
		local actor = self.actor
		anim = actor:GetComponent("Animator")
		if(anim == nil) then
			self.hasAnimator = false
		end
		
	end
}

